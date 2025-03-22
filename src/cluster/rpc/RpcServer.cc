#include <cluster/rpc/RpcClient.hpp>
#include <cluster/rpc/RpcServer.hpp>
#include <cluster/rpc/RpcSerializer.hpp>
#include <cluster/protocol/Protocol.hpp>

#define BBGENGINE_MODULE_NAME "[BBG::Node]"

cluster::RpcSerializer serializer;
using namespace cluster::protocol;
using namespace bbt::network;

namespace cluster
{


RpcServer::RpcServer(std::shared_ptr<EvThread> evthread):
    m_ev_thread(evthread)
{
}

RpcServer::~RpcServer()
{
}

int RpcServer::Register(const std::string& method, RpcCallback callback)
{
    m_registed_methods[method] = callback;
    return 0;
}

util::errcode::ErrOpt RpcServer::OnRemoteCall(bbt::core::Buffer& req)
{
    FieldValue field;
    std::string method;
    int64_t call_seq = 0;
    RpcSerializer coder;

    // 读取 mothod name
    {
        auto err = coder.DeserializeOne(req, field);
        if (err)
            return err;
        
        if (field.header.field_type != STRING)
            return util::errcode::Errcode("a bad protocol! method name must be string", util::errcode::emErr::CommonErr);

        method = std::move(field.string);
    }

    // 读取 call seq
    {
        auto err = coder.DeserializeOne(req, field);
        if (err) 
            return err;

        if (field.header.field_type != INT64)
            return util::errcode::Errcode("a bad protocol! method call_seq must be string", util::errcode::emErr::CommonErr);

        call_seq = field.value.int64_value;
    }
    

    auto iter = m_registed_methods.find(method);
    if (iter == m_registed_methods.end())
        return util::errcode::Errcode("method not found", util::errcode::emErr::CommonErr);

    return std::nullopt;
}

std::vector<std::string> RpcServer::GetRegistedMethods() const
{
    std::vector<std::string> methods;
    for (auto& iter : m_registed_methods) {
        methods.push_back(iter.first);
    }

    return methods;
}

bool RpcServer::HasMethod(const std::string& method) const
{
    return m_registed_methods.find(method) != m_registed_methods.end();
}


void RpcServer::Init(const bbt::network::IPAddress& listen_addr, const bbt::network::IPAddress& registery_addr, int timeout)
{
    m_listen_addr = listen_addr;
    m_registery_addr = registery_addr;
    m_connect_timeout = timeout;
    m_uuid = std::make_shared<util::other::Uuid>();

    m_registery_client = std::make_shared<bbt::network::TcpClient>(m_ev_thread);
    m_tcp_server = std::make_shared<bbt::network::TcpServer>(m_ev_thread);

    if (auto err = _InitRegisteryClient(); err.has_value()) {
        OnError(err.value());
        return;
    }

    if (auto err = _InitTcpServer(); err.has_value()) {
        OnError(err.value());
        return;
    }

}

util::other::Uuid::SPtr RpcServer::GetUUID() const
{
    return m_uuid;
}

NodeState RpcServer::GetNodeState()
{
    return m_state;
}

const std::string& RpcServer::GetName() const
{
    return m_service_name;
}

util::errcode::ErrOpt RpcServer::Start()
{
    m_tcp_server->AsyncListen(m_listen_addr, [weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->C2S_OnAccept(id);
    });

    m_registery_client->AsyncConnect(m_registery_addr, m_connect_timeout);

    return std::nullopt;
}

void RpcServer::Stop()
{
}


std::shared_ptr<util::other::Uuid> RpcServer::GetRandomUuidByMethod(const std::string& method)
{
    return std::make_shared<util::other::Uuid>();
}



void RpcServer::Update()
{
    if (m_registery_client) {
        if (!m_registery_client->IsConnected()) {
            if (bbt::core::clock::is_expired<bbt::core::clock::ms>(m_connect_to_registery_ms)) {
                _ConnectToRegistery();
            }
        }

        if (bbt::core::clock::is_expired<bbt::core::clock::ms>(m_last_heatbeart_ms)) {
            m_last_heatbeart_ms = bbt::core::clock::now() + bbt::core::clock::ms(m_heartbeat_timeout);
    
            if (auto err = N2R_DoHeartBeatReq(); err != std::nullopt)
                OnError(err.value());
        }
    }
}

void RpcServer::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void RpcServer::Online()
{
    m_state = NODESTATE_ONLINE;
}

void RpcServer::Clear()
{
    m_service_name = "";
    m_state = NODESTATE_UNREGISTER;
    m_uuid = nullptr;
}

util::errcode::ErrOpt RpcServer::RemoteCall(const std::string& method, bbt::core::Buffer& buffer)
{
    // 找到本地方法

    // 解析协议头

    // 执行方法
    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply)
{

    return std::nullopt;
}

void RpcServer::RequestFromRegistery(const bbt::core::Buffer& buffer)
{
    ProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.Size() < sizeof(ProtocolHead)) {
            OnError(util::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (ProtocolHead*)buffer.Peek();
        if (buffer.Size() < head->protocol_length) {
            OnError(util::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    }

    {
        if (!m_registery_connected && head->protocol_type != R2N_HANDSHAKE_RESP) {
            OnError(util::errcode::Errcode("registery not handshake", util::errcode::emErr::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = R2N_Dispatch((emR2NProtocolType)head->protocol_type, (void*)buffer.Peek(), head->protocol_length); err != std::nullopt) {
        OnError(err.value());
    }
}

void RpcServer::OnTimeoutFromRegistey(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME "timeout! conn=" + std::to_string(id));
}

void RpcServer::OnCloseFromRegistery(bbt::network::ConnId id, const bbt::network::IPAddress& addr)
{
    OnInfo("[ClusterNode] lose connect! conn=" + std::to_string(id));
    m_registery_connected = false;
    _DelayConnectToRegistery();
}

util::errcode::ErrOpt RpcServer::_InitTcpServer()
{
    m_tcp_server->SetOnSend([weak_this{weak_from_this()}](bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->C2S_OnSend(id, err, len);
    });
    m_tcp_server->SetOnRecv([weak_this{weak_from_this()}](bbt::network::ConnId id, const bbt::core::Buffer& buffer) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->C2S_OnRecv(id, buffer);
    });
    m_tcp_server->SetOnClose([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->C2S_OnClose(id);
    });
    m_tcp_server->SetOnTimeout([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->C2S_OnTimeout(id);
    });
    m_tcp_server->SetOnErr([weak_this{weak_from_this()}](const util::errcode::Errcode& err) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->OnError(err);
    });
    m_tcp_server->SetTimeout(m_connect_timeout);

    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::_InitRegisteryClient()
{
    m_registery_client->SetOnSend([weak_this{weak_from_this()}](bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2S_OnSend(id, err, len);
    });
    m_registery_client->SetOnRecv([weak_this{weak_from_this()}](bbt::network::ConnId id, const bbt::core::Buffer& buffer) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2S_OnRecv(id, buffer);
    });
    m_registery_client->SetOnClose([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2S_OnClose(id);
    });
    m_registery_client->SetOnTimeout([weak_this{weak_from_this()}](bbt::network::ConnId id) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->R2S_OnTimeout(id);
    });
    m_registery_client->SetOnConnect([weak_this{weak_from_this()}](bbt::network::ConnId id, util::errcode::ErrOpt err) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
        {
            if (err.has_value())
            {
                shared_this->OnError(err.value());
                return;
            }
            shared_this->R2S_OnConnect(id);
        }
    });
    m_registery_client->SetOnErr([weak_this{weak_from_this()}](const util::errcode::Errcode& err) {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->OnError(err);
    });
    m_registery_client->SetConnectionTimeout(BBGENGINE_CONNECT_TIMEOUT);
}

void RpcServer::_DelayConnectToRegistery()
{
    m_connect_to_registery_ms = bbt::core::clock::nowAfter(bbt::core::clock::ms{m_reconnect_time});
}

void RpcServer::_ConnectToRegistery()
{
    auto connect_to_registery_err = m_registery_client->AsyncConnect(m_registery_addr, m_connect_timeout);

    if (connect_to_registery_err != std::nullopt)
        OnError(connect_to_registery_err.value());

}

void RpcServer::OnHandshakeSucc()
{
    m_registery_connected = true;
}

#pragma region registery 网络事件

void RpcServer::R2S_OnConnect(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [R2S_OnConnect] conn=" + std::to_string(id));
    m_registery_connected = true;
    if (auto err = N2R_DoHandshakeReq(); err != std::nullopt)
        OnError(err.value());

}

void RpcServer::R2S_OnClose(bbt::network::ConnId id)
{
    OnInfo("[ClusterNode] lose connect! conn=" + std::to_string(id));
    m_registery_connected = false;
    _DelayConnectToRegistery();

}

void RpcServer::R2S_OnTimeout(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME "timeout! conn=" + std::to_string(id));
}

void RpcServer::R2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len)
{
    if (err.has_value())
        OnError(err.value());
}

void RpcServer::R2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer)
{
    RequestFromRegistery(buffer);
}


void RpcServer::C2S_OnAccept(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [C2S_OnAccept] conn=" + std::to_string(id));
}

void RpcServer::C2S_OnClose(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [C2S_OnClose] conn=" + std::to_string(id));
}

void RpcServer::C2S_OnTimeout(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME " [C2S_OnTimeout] conn=" + std::to_string(id));
}

void RpcServer::C2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len)
{
    if (err.has_value())
        OnError(err.value());
}

void RpcServer::C2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer)
{
}


#pragma endregion

#pragma region registery protocol

util::errcode::ErrOpt RpcServer::R2N_Dispatch(emR2NProtocolType type, void* proto, size_t proto_len)
{
#define CheckHelper(emProtoType, TClass, Handler) \
    case emProtoType: \
        resp = new TClass(); \
        if (!resp->ParseFromArray(proto_data, proto_data_len)) { \
            return util::errcode::Errcode(BBGENGINE_MODULE_NAME " parse protocol failed! proto=" #emProtoType, util::errcode::emErr::RPC_BAD_PROTOCOL); \
        } else \
            OnDebug(BBGENGINE_MODULE_NAME " [R2N_Dispatch] protocol=" #emProtoType); \
        return Handler(static_cast<TClass*>(resp));


    google::protobuf::Message*  resp = nullptr;
    // ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = proto_len - sizeof(ProtocolHead);

    switch (type)
    {
        CheckHelper(R2N_KEEPALIVE_RESP,             R2N_KeepAlive_Resp,             R2N_OnHeartBeatResp);
        CheckHelper(R2N_HANDSHAKE_RESP,             R2N_Handshake_Resp,             R2N_OnHandshakeResp);
        CheckHelper(R2N_REGISTER_METHOD_RESP,       R2N_RegisterMethod_Resp,        R2N_OnRegisterMethodResp);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

    delete resp;
#undef CheckHelper
}

util::errcode::ErrOpt RpcServer::R2N_OnHandshakeResp(R2N_Handshake_Resp* resp)
{
    // resp->has_head();
    int err = resp->err();
    if (err != HANDSHAKE_SUCC)
        return util::errcode::Errcode{"[ClusterNode] handshake failed!", util::errcode::CommonErr};
    else
        OnHandshakeSucc();
    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::R2N_OnHeartBeatResp(R2N_KeepAlive_Resp* resp)
{
    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::R2N_OnRegisterMethodResp(R2N_RegisterMethod_Resp* resp)
{
    if (resp == nullptr)
        return util::errcode::Errcode{"[ClusterNode] register method resp is null!", util::errcode::CommonErr};
    
    emRegisterMethodErr err = resp->err();

    if (err != emRegisterMethodErr::SUCC)
        return util::errcode::Errcode{"[ClusterNode] register method failed!", util::errcode::CommonErr};

    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::N2R_DoHandshakeReq()
{
    N2RProtocolHead*  head = new N2RProtocolHead();
    N2R_Handshake_Req req;

    head->set_uuid(m_uuid->ToByte());
    req.set_allocated_head(head);
    req.set_ip(m_listen_addr.GetIP());
    req.set_port(m_listen_addr.GetPort());

    return SendToRegistery(N2R_HANDSHAKE_REQ, bbt::core::Buffer{req.SerializeAsString().c_str(), req.ByteSizeLong()});
}

util::errcode::ErrOpt RpcServer::N2R_DoHeartBeatReq()
{
    N2RProtocolHead*  head = new N2RProtocolHead();
    N2R_KeepAlive_Req req;

    head->set_uuid(m_uuid->ToByte());
    req.set_allocated_head(head);

    return SendToRegistery(N2R_KEEPALIVE_REQ, bbt::core::Buffer{req.SerializeAsString().c_str(), req.ByteSizeLong()});
}

util::errcode::ErrOpt RpcServer::SendToRegistery(emN2RProtocolType type, const bbt::core::Buffer& proto)
{
    bbt::core::Buffer buffer;
    ProtocolHead* head = nullptr;
    
    if (m_registery_client == nullptr)
        return util::errcode::Errcode{"registery client is null!", util::errcode::CommonErr};
    
    
    buffer.WriteNull(sizeof(ProtocolHead));
    head = (ProtocolHead*)buffer.Peek();
    head->protocol_type = type;
    head->protocol_length = sizeof(ProtocolHead) + proto.Size();

    buffer.WriteString(proto.Peek(), proto.Size());

    if (auto err = m_registery_client->Send(buffer); err.has_value())
    {
        OnError(err.value());
        return err;
    }

    OnDebug(BBGENGINE_MODULE_NAME " [SendToReg] protocol=" + std::to_string(type));
    return std::nullopt;
}

util::errcode::ErrOpt RpcServer::SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    auto uuid = m_client_conn_mgr.m_rpc_client_connid_uuids.find(id);
    if (uuid == m_client_conn_mgr.m_rpc_client_connid_uuids.end())
        return util::errcode::Errcode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};

    auto client = m_client_conn_mgr.m_rpc_clients.find(uuid->second);
    if (client == nullptr)
        return util::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};


    if (auto err = client->second->Send(buffer); err.has_value())
    {
        OnError(err.value());
        return err;
    }

    return std::nullopt;
}

void RpcServer::OnSendToRegistery(util::errcode::ErrOpt err, size_t len)
{
    if (err.has_value())
        OnError(err.value());
}

util::errcode::ErrOpt RpcServer::N2N_Dispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len)
{
#define CheckHelper(emProtoType, TClass, Handler) \
    case emProtoType: \
        resp = new TClass(); \
        if (!resp->ParseFromArray(proto_data, proto_data_len)) { \
            return util::errcode::Errcode(BBGENGINE_MODULE_NAME " parse protocol failed! proto=" #emProtoType, util::errcode::emErr::RPC_BAD_PROTOCOL); \
        } else \
            OnDebug(BBGENGINE_MODULE_NAME " [R2N_Dispatch] protocol=" #emProtoType); \
        return Handler(id, static_cast<TClass*>(resp));

    google::protobuf::Message*  resp = nullptr;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = proto_len - sizeof(ProtocolHead);

    switch (type)
    {
        CheckHelper(N2N_CALL_METHOD_RESP,           N2N_CallMethod_Req,             N2N_OnCallRemoteMethod);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }
    
    delete resp;
    return std::nullopt;
#undef CheckHelper
}

util::errcode::ErrOpt RpcServer::N2N_OnCallRemoteMethod(bbt::network::ConnId id, protocol::N2N_CallMethod_Req* req)
{
    util::other::Uuid uuid;
    util::errcode::Errcode err;
    std::string params = req->params();

    if (!req->has_head())
        return util::errcode::Errcode{"[ClusterNode] call remote method req head is null!", util::errcode::CommonErr};

    if (!uuid.FromByte(req->head().uuid().c_str(), req->head().uuid().size()))
        return util::errcode::Errcode{"[ClusterNode] call remote method req uuid is invalid!", util::errcode::CommonErr};

    if (uuid != *m_uuid)
        return util::errcode::Errcode{"[ClusterNode] call remote method req uuid is not match!", util::errcode::CommonErr};

    auto method = req->method();
    if (method.empty())
        return util::errcode::Errcode{"[ClusterNode] call remote method req method is empty!", util::errcode::CommonErr};


    if (!HasMethod(method))
        return util::errcode::Errcode{"[ClusterNode] call remote method req method not found!", util::errcode::CommonErr};

    return std::nullopt;
}


util::errcode::ErrOpt RpcServer::N2R_DoRegisterMethodReq(const std::string& method, util::other::Uuid signature)
{
    N2RProtocolHead           head;
    N2R_RegisterMethod_Req    req;
    bbt::core::Buffer                   buffer;

    if (method.size() >= METHOD_NAME_MAX_LEN)
        return util::errcode::Errcode{BBGENGINE_MODULE_NAME " method name too long!", util::errcode::CommonErr};

    head.set_uuid(m_uuid->ToString());
    req.set_allocated_head(&head);
    req.set_methods(0, method);

    buffer.WriteString(req.SerializeAsString().c_str(), req.ByteSizeLong());

    return SendToRegistery(N2R_REGISTER_METHOD_REQ, buffer);
}

#pragma endregion

#undef BBGENGINE_MODULE_NAME

} // namespace cluster::rpc