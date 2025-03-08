#include <bbt/core/clock/Clock.hpp>
#include <cluster/node/ClusterNode.hpp>
#include <cluster/connection/N2RConnection.hpp>
#include <cluster/connection/R2NConnection.hpp>

#define BBGENGINE_MODULE_NAME "[BBG::Node]"

namespace cluster
{

RpcSerializer serializer;

ClusterNode::ClusterNode():
    m_rpc_clients(10),
    m_uuid(std::make_shared<util::other::Uuid>())
{
}

ClusterNode::~ClusterNode()
{
}

void ClusterNode::Init(const util::network::IPAddress& listen_addr, const util::network::IPAddress& registery_addr, int timeout)
{
    m_listen_addr = listen_addr;
    m_registery_addr = registery_addr;
    m_connect_timeout = timeout;
    
}

util::other::Uuid::SPtr ClusterNode::GetUUID() const
{
    return m_uuid;
}

NodeState ClusterNode::GetNodeState()
{
    return m_state;
}

const std::string& ClusterNode::GetName() const
{
    return m_service_name;
}

util::errcode::ErrOpt ClusterNode::Start()
{
    if (auto err = InitNetwork(); err != std::nullopt)
        return err;

    m_registery_client = std::make_shared<util::network::TcpClient>(m_network, m_registery_addr.GetIP().c_str(), m_registery_addr.GetPort(), m_connect_timeout);
    m_rpc_server = std::make_shared<RpcServer>(shared_from_this(), m_listen_addr, m_connect_timeout);
    m_tcp_server = std::make_shared<util::network::TcpServer>(m_network, m_listen_addr.GetIP(), m_listen_addr.GetPort(), m_connect_timeout);
    
    m_tcp_server->Init([node_weak_ptr{weak_from_this()}](auto libevent_conn)->std::shared_ptr<util::network::Connection>{
        auto node_shared_ptr = node_weak_ptr.lock();
        if (node_shared_ptr == nullptr)
            return nullptr;

        return std::make_shared<cluster::N2RConnection>(node_weak_ptr, libevent_conn, node_shared_ptr->m_connect_timeout);
    });
    m_tcp_server->Start();

    return std::nullopt;
}

void ClusterNode::Stop()
{
}


std::shared_ptr<util::other::Uuid> ClusterNode::GetRandomUuidByMethod(const std::string& method)
{
    return std::make_shared<util::other::Uuid>();
}



void ClusterNode::Update()
{
    // 处理和所有rpc node的心跳
    for (auto& it : m_rpc_clients) {
        if (auto other_node_conn = it.second->GetConn(); other_node_conn && !other_node_conn->IsClosed()) {
            if (auto err = N2N_DoHeartBeat(other_node_conn->GetConnId()); err != std::nullopt)
                OnError(err.value());
        }
    }

    if (m_registery_client) {
        if (auto conn = m_registery_client->GetConn(); conn == nullptr) {
            if (bbt::core::clock::is_expired<bbt::core::clock::ms>(m_connect_to_registery_ms)) {
                _ConnectToRegistery();
            }
        }
        else if(!conn->IsClosed())
        {
            if (bbt::core::clock::is_expired<bbt::core::clock::ms>(m_last_heatbeart_ms)) {
                m_last_heatbeart_ms = bbt::core::clock::now() + bbt::core::clock::ms(m_heartbeat_timeout);
        
                if (auto err = N2R_DoHeatBeatReq(); err != std::nullopt)
                    OnError(err.value());
            }
        }
    }

    // 定时获取注册中心的节点信息
    if (bbt::core::clock::is_expired<bbt::core::clock::ms>(m_last_get_nodes_info_ms) && m_registery_connected) {
        m_last_get_nodes_info_ms = bbt::core::clock::now() + bbt::core::clock::s(10);
        if (auto err = N2R_DoGetNodesInfoReq(); err != std::nullopt)
            OnError(err.value());
    }
}

void ClusterNode::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void ClusterNode::Online()
{
    m_state = NODESTATE_ONLINE;
}

void ClusterNode::Clear()
{
    m_service_name = "";
    m_state = NODESTATE_UNREGISTER;
    m_uuid = nullptr;
}

util::errcode::ErrOpt ClusterNode::RemoteCall(const std::string& method, bbt::core::Buffer& buffer)
{
    // 找到本地方法

    // 解析协议头

    // 执行方法
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply)
{

    return std::nullopt;
}

void ClusterNode::RequestFromRegistery(bbt::core::Buffer& buffer)
{
    protocol::ProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.Size() < sizeof(protocol::ProtocolHead)) {
            OnError(util::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (protocol::ProtocolHead*)buffer.Peek();
        if (buffer.Size() < head->protocol_length) {
            OnError(util::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    }

    {
        if (!m_registery_connected && head->protocol_type != protocol::R2N_HANDSHAKE_RESP) {
            OnError(util::errcode::Errcode("registery not handshake", util::errcode::emErr::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = R2N_Dispatch((protocol::emR2NProtocolType)head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt) {
        OnError(err.value());
    }
}

void ClusterNode::OnTimeoutFromRegistey(bbt::network::ConnId id)
{
    OnInfo(BBGENGINE_MODULE_NAME "timeout! conn=" + std::to_string(id));
}

void ClusterNode::OnCloseFromRegistery(bbt::network::ConnId id, const util::network::IPAddress& addr)
{
    OnInfo("[ClusterNode] lose connect! conn=" + std::to_string(id));
    m_registery_connected = false;
    m_registery_client->DelConn();
    _DelayConnectToRegistery();
}

void ClusterNode::RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
}

void ClusterNode::OnTimeoutFromNode(bbt::network::ConnId id)
{
}

void ClusterNode::OnCloseFromNode(bbt::network::ConnId id)
{
    m_rpc_client_connid_uuids.erase(id);
    m_rpc_clients.erase(m_rpc_client_connid_uuids[id]);
    m_tcp_server->DelConnect(id);
}

util::errcode::ErrOpt ClusterNode::InitNetwork()
{
    if (m_network != nullptr)
        return util::errcode::Errcode("already init network!", util::errcode::emErr::CommonErr);

    m_network = std::make_shared<bbt::network::libevent::Network>();
    auto err = m_network->AutoInitThread(2);
    if (err.has_value())
        return util::errcode::Errcode("init network failed! " + err->What(), util::errcode::emErr::CommonErr);

    return std::nullopt;
}

void ClusterNode::_DelayConnectToRegistery()
{
    m_connect_to_registery_ms = bbt::core::clock::nowAfter(bbt::core::clock::ms{m_reconnect_time});
}

void ClusterNode::_ConnectToRegistery()
{
    if (!(m_registery_client && m_registery_client->GetConn() == nullptr))
        return;

    auto connect_to_registery_err = m_registery_client->AsyncConnect(
    [weak_this{weak_from_this()}]
    (util::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn){
        if (weak_this.expired())
            return;

        auto shared_this = weak_this.lock();
        if (err.has_value()) {
            shared_this->OnError(err.value());
            return;
        }

        auto new_conn = std::make_shared<N2RConnection>(
            weak_this,
            std::static_pointer_cast<bbt::network::libevent::Connection>(conn),
            shared_this->m_connect_timeout);

        new_conn->Init();
        shared_this->m_registery_client->SetConn(new_conn);

        shared_this->N2R_DoHandshakeReq();
    });

    if (connect_to_registery_err != std::nullopt)
        OnError(connect_to_registery_err.value());

}

void ClusterNode::OnHandshakeSucc()
{
    m_registery_connected = true;
}


#pragma region registery protocol

util::errcode::ErrOpt ClusterNode::R2N_Dispatch(protocol::emR2NProtocolType type, void* proto, size_t proto_len)
{
    using namespace protocol;
#define CheckHelper(emProtoType, TClass, Handler) \
    case emProtoType: \
        resp = new TClass(); \
        if (!resp->ParseFromArray(proto, proto_len)) { \
            return util::errcode::Errcode("parse" #emProtoType "failed!", util::errcode::emErr::RPC_BAD_PROTOCOL); \
        } else \
            OnDebug(BBGENGINE_MODULE_NAME " [R2N_Dispatch] protocol=" #emProtoType); \
        return Handler(static_cast<TClass*>(resp));


    google::protobuf::Message* resp = nullptr;

    switch (type)
    {
        CheckHelper(R2N_KEEPALIVE_RESP,             R2N_KeepAlive_Resp,             R2N_OnHeartBeatResp);
        CheckHelper(R2N_HANDSHAKE_RESP,             R2N_Handshake_Resp,             R2N_OnHandshakeResp);
        CheckHelper(R2N_REGISTER_METHOD_RESP,       R2N_RegisterMethod_Resp,        R2N_OnRegisterMethodResp);
        CheckHelper(R2N_GET_NODES_INFO_RESP,        R2N_GetNodesInfo_Resp,          R2N_OnGetNodesInfoResp);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

    delete resp;
#undef EasyCheck
}

util::errcode::ErrOpt ClusterNode::R2N_OnHandshakeResp(protocol::R2N_Handshake_Resp* resp)
{
    OnHandshakeSucc();
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::R2N_OnHeartBeatResp(protocol::R2N_KeepAlive_Resp* resp)
{
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::R2N_OnRegisterMethodResp(protocol::R2N_RegisterMethod_Resp* resp)
{
    if (resp == nullptr)
        return util::errcode::Errcode{"[ClusterNode] register method resp is null!", util::errcode::CommonErr};
    
    protocol::emRegisterMethodErr err = resp->err();

    if (err != protocol::emRegisterMethodErr::SUCC)
        return util::errcode::Errcode{"[ClusterNode] register method failed!", util::errcode::CommonErr};

    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::R2N_OnGetNodesInfoResp(protocol::R2N_GetNodesInfo_Resp* resp)
{
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::N2R_DoHandshakeReq()
{
    protocol::N2RProtocolHead   head;
    protocol::N2R_Handshake_Req req;
    bbt::core::Buffer           buffer;

    head.set_uuid(m_uuid->ToString());
    req.set_allocated_head(&head);
    buffer.WriteString(req.SerializeAsString().c_str(), req.ByteSizeLong());

    return SendToRegistery(protocol::N2R_HANDSHAKE_REQ, buffer);
}

util::errcode::ErrOpt ClusterNode::N2R_DoHeatBeatReq()
{
    protocol::N2RProtocolHead   head;
    protocol::N2R_KeepAlive_Req req;
    bbt::core::Buffer           buffer;

    head.set_uuid(m_uuid->ToString());
    req.set_allocated_head(&head);
    buffer.WriteString(req.SerializeAsString().c_str(), req.ByteSizeLong());

    return SendToRegistery(protocol::N2R_HANDSHAKE_REQ, buffer);
}

util::errcode::ErrOpt ClusterNode::SendToRegistery(protocol::emN2RProtocolType type, bbt::core::Buffer& proto)
{
    protocol::ProtocolHead head;
    bbt::core::Buffer buffer;
    
    if (m_registery_client == nullptr)
        return util::errcode::Errcode{"registery client is null!", util::errcode::CommonErr};

    auto conn = m_registery_client->GetConn();
    if (conn == nullptr)
        return util::errcode::Errcode{"registery connection is null!", util::errcode::CommonErr};
    
    head.protocol_type = type;
    head.protocol_length = sizeof(protocol::ProtocolHead) + buffer.Size();

    buffer.Write(head);
    buffer.WriteString(proto.Peek(), proto.Size());

    conn->Send(buffer.Peek(), buffer.Size());
    OnDebug(BBGENGINE_MODULE_NAME " [SendToReg] protocol=" + std::to_string(type));
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    auto uuid = m_rpc_client_connid_uuids.find(id);
    if (uuid == m_rpc_client_connid_uuids.end())
        return util::errcode::Errcode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};

    auto client = m_rpc_clients.find(uuid->second);
    if (client == nullptr)
        return util::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    auto conn = client->second->GetConn();
    if (conn == nullptr)
        return util::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.Size());
    return std::nullopt;
}

void ClusterNode::OnSendToRegistery(util::errcode::ErrOpt err, size_t len)
{
    if (err.has_value())
        OnError(err.value());
}

util::errcode::ErrOpt ClusterNode::N2N_Dispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len)
{
    /**
     * 保证Dispatch时是线程安全的
     */

    
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::N2N_DoHeartBeat(bbt::network::ConnId id)
{
    N2N_KeepAlive_Req req;
    OnDebug("send n2n heartbeat!");
    auto it = m_rpc_client_connid_uuids.find(id);
    if (it == m_rpc_client_connid_uuids.end())
        return util::errcode::Errcode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};
    
    auto conn = m_rpc_clients.find(it->second);
    if (conn == m_rpc_clients.end())
        return util::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    req.head.protocol_type = N2N_KEEPALIVE_REQ;
    req.head.protocol_length = sizeof(N2N_KeepAlive_Req);
    req.head.timestamp_ms = bbt::core::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return util::errcode::Errcode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};
    
    conn->second->GetConn()->Send((char*)&req, sizeof(req));
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::N2R_DoRegisterMethodReq(const std::string& method, util::other::Uuid signature)
{
    protocol::N2RProtocolHead           head;
    protocol::N2R_RegisterMethod_Req    req;
    bbt::core::Buffer                   buffer;

    if (method.size() >= protocol::METHOD_NAME_MAX_LEN)
        return util::errcode::Errcode{BBGENGINE_MODULE_NAME " method name too long!", util::errcode::CommonErr};

    head.set_uuid(m_uuid->ToString());
    req.set_allocated_head(&head);
    req.set_methods(0, method);

    buffer.WriteString(req.SerializeAsString().c_str(), req.ByteSizeLong());

    return SendToRegistery(protocol::N2R_REGISTER_METHOD_REQ, buffer);
}

util::errcode::ErrOpt ClusterNode::N2R_DoGetNodesInfoReq()
{
    protocol::N2RProtocolHead           head;
    protocol::N2R_GetNodesInfo_Req      req;
    bbt::core::Buffer                   buffer;

    head.set_uuid(m_uuid->ToString());
    req.set_allocated_head(&head);
    buffer.WriteString(req.SerializeAsString().c_str(), req.ByteSizeLong());
    
    return SendToRegistery(protocol::N2R_GET_NODES_INFO_REQ, buffer);
}


#pragma endregion

}

#undef BBGENGINE_MODULE_NAME