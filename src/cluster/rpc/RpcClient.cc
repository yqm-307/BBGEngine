#include <cluster/rpc/RpcClient.hpp>
#include <random>

using namespace cluster::protocol;
using namespace bbt::core::clock;


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

RpcClient::RpcClient():
    m_network(std::make_shared<bbt::network::libevent::Network>())
{
}

util::errcode::ErrOpt RpcClient::Init(const util::network::IPAddress& registery_addr, int timeout_ms)
{
    
}

void RpcClient::Start()
{
    m_registery_client = std::make_shared<util::network::TcpClient>(m_network);
    m_registery_client->Init([weak_this{weak_from_this()}](bbt::network::libevent::ConnectionSPtr conn) -> std::shared_ptr<util::network::Connection> {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            return std::make_shared<RpcConnection<RpcClient>>(RPC_CONN_TYPE_CR, shared_this, conn, BBGENGINE_CLUSTER_CONN_FREE_TIMEOUT);

        return nullptr;
    });

    m_registery_client->AsyncConnect([weak_this{weak_from_this()}](util::errcode::ErrOpt err, std::shared_ptr<util::network::TcpClient> client){
        if (auto shared_this = weak_this.lock(); shared_this != nullptr && err.has_value())
            shared_this->OnError(err.value());
    });
}

void RpcClient::Stop()
{

}

void RpcClient::Update()
{
    auto registery_conn = m_registery_client->GetConn();
    if (registery_conn && !registery_conn->IsClosed())
    {
        if (is_expired<ms>(m_cache_last_update_time + ms(m_cache_update_interval)))
        {
            m_cache_last_update_time = now();
            DoGetNodesInfoReq();
        }
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

std::shared_ptr<util::network::Connection> RpcClient::GetServiceConn(const std::string& method)
{
    std::lock_guard<std::mutex> lock{m_net_mtx};

    auto uuid = m_node_cache.GetUuid(m_serializer.GetMethodHash(method));
    if (!uuid.has_value())
        return nullptr;
    
    auto it = m_server_mgr.m_rpc_clients.find(uuid.value());
    if (it == m_server_mgr.m_rpc_clients.end())
    {
        return nullptr;
    }

    return it->second->GetConn();
}


void RpcClient::SubmitReq2Listener(bbt::network::ConnId id, emRpcConnType type, bbt::core::Buffer& buffer)
{
    ProtocolHead* head = (ProtocolHead*)buffer.Peek();

    switch (type)
    {
    case RPC_CONN_TYPE_CR:
        _R_Dispatch(id, buffer.Peek(), head->protocol_length);
        break;
    case RPC_CONN_TYPE_CS:
        _S_Dispatch(id, buffer.Peek(), head->protocol_length);
        break;
    default:
        OnError(util::errcode::Errcode{"[RpcClient::SubmitReq2Listener] unknown conn type", util::errcode::CommonErr});
        break;
    }
}

void RpcClient::NotifySend2Listener(bbt::network::ConnId id, emRpcConnType type, util::errcode::ErrOpt err, size_t len)
{

}

void RpcClient::NotityOnClose2Listener(bbt::network::ConnId id, emRpcConnType type)
{

}

void RpcClient::NotityOnTimeout2Listener(bbt::network::ConnId id, emRpcConnType type)
{

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

    auto conn = m_registery_client->GetConn();
    if (!conn || (conn && conn->IsClosed()))
        return util::errcode::Errcode{"[RpcClient::_SendToRegistery] conn is null", util::errcode::CommonErr};

    buffer.WriteNull(sizeof(ProtocolHead));
    head = (ProtocolHead*)buffer.Peek();
    head->protocol_type = type;
    head->protocol_length = sizeof(ProtocolHead) + proto.Size();

    buffer.WriteString(proto.Peek(), proto.Size());

    conn->Send(buffer.Peek(), buffer.Size());
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

util::errcode::ErrOpt RpcClient::DoGetNodesInfoReq()
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