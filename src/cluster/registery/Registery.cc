#include <cluster/registery/Registery.hpp>
#include <cluster/registery/NodeMgr.hpp>

#define BBGENGINE_MODULE_NAME "[BBG::Registery]"

#define CheckHelper(emProtoType, TClass, Handler) \
    case emProtoType: \
        resp = new TClass(); \
        if (!resp->ParseFromArray(proto_data, proto_data_len)) { \
            return util::errcode::Errcode("parse protocol failed! proto=" #emProtoType, util::errcode::emErr::RPC_BAD_PROTOCOL); \
        } else \
            OnDebug(BBGENGINE_MODULE_NAME "[R2N_Dispatch] protocol=" #emProtoType); \
        return Handler(id, head, static_cast<TClass*>(resp));

using namespace cluster::protocol;
using namespace bbt::network;

namespace cluster
{

Registery::Registery():
    m_node_mgr(std::make_shared<NodeMgr>())
{
};

Registery::~Registery()
{
};

void Registery::Init(
    const IPAddress& rs_listen,
    const IPAddress& rc_listen,
    std::shared_ptr<TcpServer> rs_server,
    std::shared_ptr<TcpServer> rc_server,
    int timeout_ms)
{
    Assert(rs_server != nullptr);
    Assert(rc_server != nullptr);

    m_rs_listen_addr = rs_listen;
    m_rc_listen_addr = rc_listen;

    m_rc_server = rc_server;
    m_rs_server = rs_server;

    _InitRCServer();
    _InitRSServer();
}

void Registery::Start()
{
    m_rs_server->AsyncListen(m_rs_listen_addr, [weak_this{weak_from_this()}](bbt::network::ConnId connid)
    {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->NotifyOnAccept(connid);
    });

    m_rc_server->AsyncListen(m_rc_listen_addr, [weak_this{weak_from_this()}](bbt::network::ConnId connid)
    {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->NotifyOnAccept(connid);
    });
}

void Registery::Stop()
{
    m_rs_server->StopListen();
    m_rc_server->StopListen();
}

void Registery::Update()
{
    m_node_mgr->Update();
    // 如果node长时间没有heartbeat，就认为它已经下线，关闭连接
}

NodeRegInfo::SPtr Registery::GetNodeRegInfo(const util::other::Uuid& uuid)
{
    return m_node_mgr->GetNodeInfo(uuid);
}

void Registery::CloseConn(bbt::network::ConnId connid)
{
    m_rs_server->Close(connid);
}

NodeState Registery::GetNodeStatus(const util::other::Uuid& uuid) const
{
    return m_node_mgr->GetNodeState(uuid);
}

#pragma region 节点管理

void Registery::RegisterNode(const bbt::network::IPAddress& addr, const util::other::Uuid& uuid)
{
    auto node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return;

    node_info->SetStatus(NodeState::NODESTATE_ONLINE);
}

void Registery::UnRegisterNode(const util::other::Uuid& uuid)
{
    auto node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return;

    node_info->SetStatus(NodeState::NODESTATE_OFFLINE);
}

#pragma endregion

util::errcode::ErrOpt Registery::SendToNode(emR2NProtocolType type, const util::other::Uuid& uuid, const bbt::core::Buffer& proto)
{
    ProtocolHead*       head = nullptr;
    bbt::core::Buffer   buffer;

    auto node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return util::errcode::Errcode(BBGENGINE_MODULE_NAME " node not found! uuid=" + uuid.ToString(), util::errcode::emErr::RPC_NOT_FOUND_NODE);

    auto connid = GetConnIdByUuid(uuid);
    if (connid == 0)
        return util::errcode::Errcode(BBGENGINE_MODULE_NAME " connid not found! uuid=" + uuid.ToString(), util::errcode::emErr::RPC_NOT_FOUND_NODE);

    buffer.WriteNull(sizeof(ProtocolHead));
    head = (ProtocolHead*)buffer.Peek();
    head->protocol_type = type;
    head->protocol_length = sizeof(ProtocolHead) + proto.Size();

    buffer.WriteString(proto.Peek(), proto.Size());

    if (auto err = m_rs_server->Send(connid, buffer); err != std::nullopt)
        return err;

    OnDebug(BBGENGINE_MODULE_NAME "[SendToNode] protocol=" + std::to_string(type));
    return std::nullopt;
}

util::errcode::ErrOpt Registery::SendToNode(emR2NProtocolType type, const bbt::network::ConnId& connid, const bbt::core::Buffer& buffer)
{
    return util::errcode::Errcode{"not implement!", util::errcode::CommonErr};
}


void Registery::OnSendToNode(util::errcode::ErrOpt err, size_t len)
{
    if (err != std::nullopt)
        OnError(err.value());
}

void Registery::NotifyOnAccept(bbt::network::ConnId connid)
{
    AddHalfConn(connid);
    OnInfo(BBGENGINE_MODULE_NAME " accept connection! conn=" + std::to_string(connid));
}

void Registery::_InitRCServer()
{
    m_rc_server->Init();
    m_rc_server->SetOnErr(
        [weak_this{weak_from_this()}](const util::errcode::Errcode& err)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->OnError(err);
        }
    );
    m_rc_server->SetOnTimeout(
        [weak_this{weak_from_this()}](bbt::network::ConnId connid)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->RC_OnTimeout(connid);
        }
    );
    m_rc_server->SetOnClose(
        [weak_this{weak_from_this()}](bbt::network::ConnId connid)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->RC_OnClose(connid);
        }
    );
    m_rc_server->SetOnRecv(
        [weak_this{weak_from_this()}](bbt::network::ConnId connid, const bbt::core::Buffer& buffer)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->RC_OnRecv(connid, buffer);
        }
    );
    m_rc_server->SetOnSend(
        [weak_this{weak_from_this()}](bbt::network::ConnId connid, util::errcode::ErrOpt err, size_t len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->RC_OnSend(connid, err, len);
        }
    );


}

void Registery::_InitRSServer()
{

}

bool Registery::IsHalfConn(bbt::network::ConnId connid)
{
    std::lock_guard<std::mutex> lock(m_half_connect_set_lock);
    return m_half_connect_set.find(connid) != m_half_connect_set.end();
}

void Registery::DelHalfConn(bbt::network::ConnId connid)
{
    std::lock_guard<std::mutex> lock(m_half_connect_set_lock);
    m_half_connect_set.erase(connid);
}

void Registery::AddHalfConn(bbt::network::ConnId connid)
{
    std::lock_guard<std::mutex> lock(m_half_connect_set_lock);
    m_half_connect_set.insert(connid);
}

bbt::network::ConnId Registery::GetConnIdByUuid(const util::other::Uuid& uuid) const
{
    auto node = m_node_mgr->GetNodeInfo(uuid);
    if (node == nullptr)
        return 0;

    return node->GetConnId();
}

void Registery::OnRequest(bbt::network::ConnId connid, bbt::core::Buffer& buffer)
{
    ProtocolHead* head = nullptr;

    auto conn = m_rs_server->GetConnectById(connid);
    if (conn == nullptr) {
        OnError(util::errcode::Errcode("connection not found!", util::errcode::emErr::RPC_NOT_FOUND_NODE));
        return;
    }

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

        // 半连接只能接收握手协议
        if (IsHalfConn(connid) && head->protocol_type != N2R_HANDSHAKE_REQ) {
            OnError(util::errcode::Errcode("half-connection can only recv handshake protocol", util::errcode::emErr::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = N2RDispatch(connid, (emN2RProtocolType)head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt)
        OnError(err.value());
}

#pragma region RpcClient 网络事件

void Registery::RC_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len)
{}

void Registery::RC_OnClose(bbt::network::ConnId id)
{}

void Registery::RC_OnTimeout(bbt::network::ConnId id)
{}

void Registery::RC_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer)
{}


#pragma endregion

#pragma region RpcServer 网络事件

void Registery::RS_OnSend(ConnId id)
{}

void Registery::RS_OnClose(ConnId id)
{}

void Registery::RS_OnTimeout(ConnId id)
{}

void Registery::RS_OnRecv(ConnId id, bbt::core::Buffer& buffer)
{}


#pragma endregion

#pragma region 连接管理

void Registery::SubmitReq2Listener(bbt::network::ConnId id, emRpcConnType type, bbt::core::Buffer& buffer)
{
    switch (type)
    {
    case RPC_CONN_TYPE_RN:
        OnRequest(id, buffer);
        break;
    
    default:
        Assert(false);
        break;
    }
}

void Registery::NotifySend2Listener(bbt::network::ConnId id, emRpcConnType type, util::errcode::ErrOpt err, size_t len)
{
    switch (type)
    {
    case RPC_CONN_TYPE_RN:
        OnSendToNode(err, len);
        break;
    
    default:
        Assert(false);
        break;
    }
}

void Registery::NotityOnClose2Listener(bbt::network::ConnId connid, emRpcConnType type)
{
    Assert(type == RPC_CONN_TYPE_RN);

    util::other::Uuid uuid;
    DelHalfConn(connid);
    m_rs_server->DelConnect(connid);
    
    auto node_info = m_node_mgr->GetNodeInfo(connid);
    if (node_info != nullptr) {
        m_node_mgr->NodeOffline(connid);
        uuid = node_info->GetUuid();
    }
    
    OnInfo(BBGENGINE_MODULE_NAME " lose connection! uuid=" + uuid.ToString());

}

void Registery::NotityOnTimeout2Listener(bbt::network::ConnId connid, emRpcConnType type)
{
    Assert(RPC_CONN_TYPE_RN == type);
    OnInfo(BBGENGINE_MODULE_NAME " node timeout! conn=" + std::to_string(connid));
}

#pragma endregion

#pragma region 协议处理

util::errcode::ErrOpt Registery::N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len)
{
    google::protobuf::Message*  resp = nullptr;
    ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = proto_len - sizeof(ProtocolHead);

    switch (type)
    {
        CheckHelper(N2R_KEEPALIVE_REQ, N2R_KeepAlive_Req, OnHeartBeat);
        CheckHelper(N2R_HANDSHAKE_REQ, N2R_Handshake_Req, OnHandshake);
        CheckHelper(N2R_REGISTER_METHOD_REQ, N2R_RegisterMethod_Req, OnRegisterMethod);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

    delete resp;
    return std::nullopt;
}

util::errcode::ErrOpt Registery::OnHeartBeat(bbt::network::ConnId id, ProtocolHead* head, N2R_KeepAlive_Req* req)
{
    R2N_KeepAlive_Resp  resp;
    R2NProtocolHead*    r2n_head = new R2NProtocolHead();
    util::other::Uuid   uuid;

    if (!req->has_head())
        return util::errcode::Errcode("head not found!", util::errcode::emErr::RPC_BAD_PROTOCOL);

    Assert(uuid.FromByte(req->head().uuid().c_str(), req->head().uuid().size()));
    auto node_info = m_node_mgr->GetNodeInfo(uuid);

    if (node_info == nullptr)
        return util::errcode::Errcode("[OnHeartBeat] node not registed!", util::errcode::emErr::RPC_NOT_FOUND_NODE);

    node_info->OnHeartBeat();

    r2n_head->set_uuid(uuid.ToByte());
    resp.set_allocated_head(r2n_head);

    return SendToNode(R2N_KEEPALIVE_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
}

util::errcode::ErrOpt Registery::OnHandshake(bbt::network::ConnId id, ProtocolHead* head, N2R_Handshake_Req* req)
{
    R2NProtocolHead* r2n_head = new R2NProtocolHead();
    R2N_Handshake_Resp resp;
    util::other::Uuid uuid;

    if (!req->has_head())
        return util::errcode::Errcode("head not found!", util::errcode::emErr::RPC_BAD_PROTOCOL);

    auto info = std::make_shared<NodeRegInfo>();
    uuid.FromByte(req->head().uuid().c_str(), req->head().uuid().size());
    resp.set_err(emHandshakeErr::HANDSHAKE_FAILED);
    r2n_head->set_uuid(uuid.ToByte());
    resp.set_allocated_head(r2n_head);

    if (!IsHalfConn(id))
        return SendToNode(R2N_HANDSHAKE_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});

    if (GetNodeStatus(uuid) == NodeState::NODESTATE_ONLINE)
        return SendToNode(R2N_HANDSHAKE_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
    
    info->Init(uuid, bbt::network::IPAddress{req->ip(), req->port()});
    info->SetConnId(id);
    info->SetStatus(NodeState::NODESTATE_ONLINE);
    DelHalfConn(id);
    m_node_mgr->RegisterNode(uuid, info);
    resp.set_err(emHandshakeErr::HANDSHAKE_SUCC);

    return SendToNode(R2N_HANDSHAKE_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
}

util::errcode::ErrOpt Registery::OnRegisterMethod(bbt::network::ConnId id, ProtocolHead* head, N2R_RegisterMethod_Req* req)
{
    R2N_RegisterMethod_Resp resp;
    util::other::Uuid uuid;

    if (!req->has_head())
        return util::errcode::Errcode("head not found!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    
    Assert(uuid.FromByte(req->head().uuid().c_str(), req->head().uuid().size()));

    auto node_info = m_node_mgr->GetNodeInfo(id);
    if (node_info == nullptr) {
        resp.set_err(emRegisterMethodErr::FAILED);
        return SendToNode(R2N_REGISTER_METHOD_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
    }

    // 注册方法
    auto& methods = req->methods();
    for (int i = 0; i < methods.size(); i++) {
        if (auto err = node_info->AddMethod(methods.Get(i)); err != std::nullopt) {
            resp.set_err(emRegisterMethodErr::FAILED);
            return SendToNode(R2N_REGISTER_METHOD_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
        }
    }

    resp.set_err(emRegisterMethodErr::SUCC);
    return SendToNode(R2N_REGISTER_METHOD_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
}

util::errcode::ErrOpt Registery::C2RDispatch(bbt::network::ConnId id, protocol::emC2RProtocolType type, void* proto, size_t proto_len)
{
    google::protobuf::Message*  resp = nullptr;
    ProtocolHead*               head = (ProtocolHead*)proto;
    void*                       proto_data = (char*)proto + sizeof(ProtocolHead);
    size_t                      proto_data_len = proto_len - sizeof(ProtocolHead);

    switch (type)
    {
        CheckHelper(C2R_GET_NODES_INFO_REQ, C2R_GetNodesInfo_Req, OnGetNodesInfo);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

    delete resp;
    return std::nullopt;
}


util::errcode::ErrOpt Registery::OnGetNodesInfo(bbt::network::ConnId id, ProtocolHead* head, C2R_GetNodesInfo_Req* req)
{
    R2C_GetNodesInfo_Resp resp;
    NodeInfo info;
    util::other::Uuid uuid;

    if (!req->has_head())
        return util::errcode::Errcode("head not found!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    
    Assert(uuid.FromByte(req->head().uuid().c_str(), req->head().uuid().size()));
    
    auto nodes = m_node_mgr->GetAllNodeInfo();
    for (auto&& node : nodes) {
        NodeInfo* nodeinfo = resp.add_nodes();

        nodeinfo->set_ip(node->GetNodeAddr().GetIP());
        nodeinfo->set_port(node->GetNodeAddr().GetPort());
        nodeinfo->set_uuid(node->GetUuid().ToByte());
        for (auto&& method : node->GetMethods())
            nodeinfo->add_methods(method);
    }

    printf("%s\n", resp.DebugString().c_str());

    return SendToNode(R2N_GET_NODES_INFO_RESP, uuid, bbt::core::Buffer{resp.SerializeAsString().c_str(), resp.ByteSizeLong()});
}


}

#undef BBGENGINE_MODULE_NAME
#undef EasyCheck
