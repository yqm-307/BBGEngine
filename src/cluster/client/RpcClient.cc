#include <cluster/client/RpcClient.hpp>
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
            OnDebug(BBGENGINE_MODULE_NAME "[Dispatch] protocol=" #emProtoType); \
        return Handler(connid, head, static_cast<TClass*>(resp));


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

}

void RpcClient::Update()
{
    if (m_registery_client && m_registery_client->IsConnected())
    {
        // 定时去注册中心获取节点信息
        if (is_expired<ms>(m_cache_last_update_time + ms(m_cache_update_interval)))
        {
            m_cache_last_update_time = now();
            DoGetNodesInfo();
        }
    }
    else
    {
        if (is_expired<ms>(m_connect_to_registery_ms))
        {
            _ConnectToRegistery();
            _DelayConnectToRegistery();
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

ConnId RpcClient::GetServiceConn(const std::string& method)
{
    return m_service_mgr.GetServiceConnIdRandom(method);
}

util::errcode::ErrOpt RpcClient::_InitRegisteryClient()
{
    m_registery_client->Init();
    m_registery_client->SetConnectionTimeout(BBGENGINE_CLUSTER_CONN_FREE_TIMEOUT);
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

void RpcClient::_DelayConnectToRegistery()
{
    m_connect_to_registery_ms = bbt::core::clock::nowAfter(bbt::core::clock::ms{m_reconnect_time});
}

util::errcode::ErrOpt RpcClient::_ConnectToRegistery()
{
    return m_registery_client->AsyncConnect(m_registery_addr, BBGENGINE_CONNECT_TIMEOUT);
}

util::errcode::ErrOpt RpcClient::_SendToServer(bbt::network::ConnId connid, const bbt::core::Buffer& buffer)
{
    
    return Errcode{"_SendToNode not implementation", CommonErr};
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

#pragma region 网络事件

void RpcClient::R2C_OnConnect(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [R2C_OnConnect] conn=" + std::to_string(id));
}

void RpcClient::R2C_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len)
{
    if (err.has_value())
        OnError(err.value());
}

void RpcClient::R2C_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer)
{
    if (auto err = R2C_Dispatch(id, buffer.Peek(), buffer.Size()); err.has_value())
        OnError(err.value());
}

void RpcClient::R2C_OnClose(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [R2C_OnClose] conn=" + std::to_string(id));
}

void RpcClient::R2C_OnTimeout(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [R2C_OnTimeout] conn=" + std::to_string(id));
}

#pragma endregion

#pragma region register 协议处理

util::errcode::ErrOpt RpcClient::R2C_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    google::protobuf::Message*  resp = nullptr;
    ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = len - sizeof(ProtocolHead);
    emR2CProtocolType           type = (emR2CProtocolType)head->protocol_type;

    switch (type)
    {
        CheckHelper(R2C_GET_NODES_INFO_RESP, R2C_GetNodesInfo_Resp, R2C_OnGetNodeInfo);
    default:
        return util::errcode::Errcode{"[RpcClient::_R_Dispatch] unknown protocol type=" + std::to_string(type), util::errcode::CommonErr};
    }

    return std::nullopt;
}
util::errcode::ErrOpt RpcClient::R2C_OnGetNodeInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::R2C_GetNodesInfo_Resp* resq)
{
}

util::errcode::ErrOpt RpcClient::DoGetNodesInfo()
{
    C2RProtocolHead* head = new C2RProtocolHead();
    C2R_GetNodesInfo_Req req;
    util::other::Uuid uuid;

    head->set_uuid(uuid.ToByte());
    req.set_allocated_head(head);

    return _SendToRegistery(C2R_GET_NODES_INFO_REQ, bbt::core::Buffer{req.SerializeAsString().c_str(), req.ByteSizeLong()});
}

util::errcode::ErrOpt RpcClient::S2C_OnRemoteCall(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::S2C_CallMethod_Resp* req)
{
    if (req->err() != emC2SCallMethodErr::CALL_METHOD_SUCC)
    {
        return Errcode{"[RpcClient::_OnGetNodeInfo] call method failed! err=" + std::to_string(req->err()), CommonErr};
    }

    return Errcode{"_OnGetNodeInfo not implementation", CommonErr};
}

util::errcode::ErrOpt RpcClient::C2S_SendRemoteCall(bbt::network::ConnId id, const std::string& method, const bbt::core::Buffer& params)
{
    C2S_CallMethod_Req req;
    req.set_method(method);
    req.set_params(params.Peek(), params.Size());

    return _SendToServer(id, bbt::core::Buffer{req.SerializeAsString().c_str(), req.ByteSizeLong()});
}

#pragma endregion


#pragma region RpcServer 协议处理

util::errcode::ErrOpt RpcClient::S2C_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    google::protobuf::Message*  resp = nullptr;
    ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = len - sizeof(ProtocolHead);
    emR2CProtocolType           type = (emR2CProtocolType)head->protocol_type;

    switch (type)
    {
        CheckHelper(S2C_CALL_METHOD_RESP, S2C_CallMethod_Resp, S2C_OnRemoteCall);
    default:
        return util::errcode::Errcode{"[RpcClient::_R_Dispatch] unknown protocol type=" + std::to_string(type), util::errcode::CommonErr};
    }

    return std::nullopt;
}

#pragma endregion

} // namespace plugin::ecs::rpc


#undef BBGENGINE_MODULE_NAME
#undef CheckHelper