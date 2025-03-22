#include <cluster/rpc/RpcClient.hpp>
#include <random>

using namespace cluster::protocol;
using namespace bbt::core::clock;
using namespace util::errcode;
using namespace bbt::network;

typedef bbt::core::Buffer Buffer;


namespace cluster
{

#define BBGENGINE_MODULE_NAME "[BBG::RpcClient]"

#define CheckHelper(emProtoType, TClass, Handler) \
    case emProtoType: \
        resp = new TClass(); \
        if (!resp->ParseFromArray(proto_data, proto_data_len)) { \
            return util::errcode::Errcode("parse protocol failed! proto=" #emProtoType, util::errcode::emErr::RPC_BAD_PROTOCOL); \
        } else \
            OnDebug(BBGENGINE_MODULE_NAME "[R2N_Dispatch] protocol=" #emProtoType); \
        return Handler(connid, head, static_cast<TClass*>(resp));

void NodeCache::UpdateCache(const util::other::Uuid& uuid, const std::vector<RpcMethodHash>& methods)
{
    m_method_uuids.clear();

    for (auto& method : methods)
    {
        m_method_uuids[method].insert(uuid);
        m_node_methods[uuid].insert(method);
    }
}

void NodeCache::DeleteCache(const util::other::Uuid& uuid)
{
    auto iter = m_node_methods.find(uuid);
    if (iter == m_node_methods.end())
    {
        return;
    }

    for (auto& method : iter->second)
    {
        m_method_uuids[method].erase(uuid);
    }

    m_node_methods.erase(iter);
}

std::optional<util::other::Uuid> NodeCache::GetUuid(RpcMethodHash method)
{
    auto iter = m_method_uuids.find(method);
    if (iter == m_method_uuids.end())
    {
        return std::nullopt;
    }

    if (iter->second.empty())
    {
        return std::nullopt;
    }

    // 随机返回一个
    std::vector<util::other::Uuid> uuids(iter->second.begin(), iter->second.end());
    util::other::Uuid random_uuid;
    std::sample(uuids.begin(), uuids.end(), &random_uuid, 1, std::mt19937{std::random_device{}()});

    return random_uuid;
}


RpcClient::~RpcClient()
{
}

RpcClient::RpcClient(std::shared_ptr<bbt::network::EvThread> evthread):
    m_ev_thread(evthread)
{
    Assert(evthread != nullptr);
}

util::errcode::ErrOpt RpcClient::Init(const bbt::network::IPAddress& registery_addr, int timeout_ms)
{
    m_registery_addr = registery_addr;
    m_registery_client = std::make_shared<bbt::network::TcpClient>(m_ev_thread);

    _InitRegisteryClient();

    return std::nullopt;
}

void RpcClient::Start()
{
    _ConnectToRegistery();
}

void RpcClient::Stop()
{
    if (m_registery_client)
    {
        m_registery_client->Close();
        m_registery_client = nullptr;
    }

    m_server_mgr.m_rpc_clients.clear();
}

void RpcClient::Update()
{
    if (m_registery_client && m_registery_client->IsConnected())
    {
        // 定时去注册中心获取节点信息
        if (is_expired<ms>(m_cache_last_update_time + ms(m_cache_update_interval)))
        {
            m_cache_last_update_time = now();
            _DoGetNodesInfoReq();
        }
    }
    else
    {
        _ConnectToRegistery();
    }
}

void RpcClient::OnReply(const char* data, size_t size)
{
    bbt::core::Buffer buffer(data, size);
    FieldHeader header;
    if (buffer.Size() < sizeof(header))
    {
        return;
    }

    buffer.ReadString((char*)&header, sizeof(header));
    if (header.field_type != INT64)
    {
        return;
    }

    if (buffer.Size() < header.field_len)
    {
        return;
    }

    int64_t seq;
    buffer.Read(seq);

    auto iter = m_callbacks.find(seq);
    if (iter == m_callbacks.end())
    {
        return;
    }

    auto err = iter->second(buffer);
    if (err)
    {
    }
}

#pragma region 连接管理

ConnId RpcClient::GetServiceConn(const std::string& method)
{
    std::lock_guard<std::mutex> lock{m_net_mtx};

    auto uuid = m_node_cache.GetUuid(m_serializer.GetMethodHash(method));
    if (!uuid.has_value())
        return 0;
    
    auto it = m_server_mgr.m_rpc_clients.find(uuid.value());
    if (it == m_server_mgr.m_rpc_clients.end())
    {
        return 0;
    }

    if (it->second == nullptr)
    {
        return 0;
    }

    return it->second->GetConnId();
}

util::errcode::ErrOpt RpcClient::_InitRegisteryClient()
{
    m_registery_client->SetConnectionTimeout(BBGENGINE_CONNECT_TIMEOUT);
    m_registery_client->SetOnErr([weak_this{weak_from_this()}](const util::errcode::Errcode& err) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->OnError(err);
    });
    m_registery_client->SetOnSend([weak_this{weak_from_this()}](ConnId id, ErrOpt err, size_t len) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2C_OnSend(id, err, len);
    });
    m_registery_client->SetOnRecv([weak_this{weak_from_this()}](bbt::network::ConnId id, const Buffer& buffer) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2C_OnRecv(id, buffer);
    });
    m_registery_client->SetOnClose([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2C_OnClose(id);
    });
    m_registery_client->SetOnTimeout([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2C_OnTimeout(id);
    });
    m_registery_client->SetOnConnect([weak_this{weak_from_this()}](bbt::network::ConnId id, ErrOpt err) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
        {
            if (err.has_value())
            {
                shared_this->OnError(err.value());
                return;
            }
            shared_this->R2C_OnConnect(id);
        }
    });

    return std::nullopt;
}

util::errcode::ErrOpt RpcClient::_ConnectToRegistery()
{
    return m_registery_client->AsyncConnect(m_registery_addr, BBGENGINE_CONNECT_TIMEOUT);
}

util::errcode::ErrOpt RpcClient::_SendToNode(bbt::network::ConnId connid, const bbt::core::Buffer& buffer)
{

}

util::errcode::ErrOpt RpcClient::_SendToRegistery(emC2RProtocolType type, const bbt::core::Buffer& proto)
{
    bbt::core::Buffer buffer;
    ProtocolHead* head = nullptr;

    if (m_registery_client == nullptr)
        return util::errcode::Errcode{BBGENGINE_MODULE_NAME " registery client is null!", util::errcode::CommonErr};

    buffer.WriteNull(sizeof(ProtocolHead));
    head = (ProtocolHead*)buffer.Peek();
    head->protocol_type = type;
    head->protocol_length = sizeof(ProtocolHead) + proto.Size();

    buffer.WriteString(proto.Peek(), proto.Size());

    if (auto err = m_registery_client->Send(buffer); err)
        return err;

    OnDebug(BBGENGINE_MODULE_NAME " [SendToReg] protocol=" + std::to_string(type));
    return std::nullopt;
}

#pragma endregion

#pragma region 协议处理

util::errcode::ErrOpt RpcClient::_R_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    google::protobuf::Message*  resp = nullptr;
    ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = len - sizeof(ProtocolHead);
    emR2CProtocolType           type = (emR2CProtocolType)head->protocol_type;

    switch (type)
    {
        CheckHelper(R2C_GET_NODES_INFO_RESP, R2C_GetNodesInfo_Resp, _OnGetNodeInfo);
    default:
        return util::errcode::Errcode{"[RpcClient::_R_Dispatch] unknown protocol type=" + std::to_string(type), util::errcode::CommonErr};
    }

    return std::nullopt;
}

util::errcode::ErrOpt RpcClient::_DoGetNodesInfoReq()
{
    C2RProtocolHead* head = new C2RProtocolHead();
    C2R_GetNodesInfo_Req req;
    util::other::Uuid uuid;

    head->set_uuid(uuid.ToByte());
    req.set_allocated_head(head);

    return _SendToRegistery(C2R_GET_NODES_INFO_REQ, bbt::core::Buffer{req.SerializeAsString().c_str(), req.ByteSizeLong()});
}

util::errcode::ErrOpt RpcClient::_OnGetNodeInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::R2C_GetNodesInfo_Resp* req)
{

}

util::errcode::ErrOpt RpcClient::_S_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    return std::nullopt;
}

#pragma endregion

} // namespace plugin::ecs::rpc


#undef BBGENGINE_MODULE_NAME
#undef CheckHelper