#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNode.hpp>
#include <cluster/connection/N2RConnection.hpp>
#include <cluster/connection/R2NConnection.hpp>

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

void ClusterNode::Init(const bbt::net::IPAddress& listen_addr, const bbt::net::IPAddress& registery_addr, int timeout)
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

bbt::errcode::ErrOpt ClusterNode::Start()
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
            if (bbt::clock::is_expired<bbt::clock::ms>(m_connect_to_registery_ms)) {
                // m_connect_to_registery_ms = bbt::clock::gettime() + m_connect_timeout;
                _ConnectToRegistery();
            }
        }
        else if(!conn->IsClosed())
        {
            if (bbt::clock::is_expired<bbt::clock::ms>(m_last_heatbeart_ms)) {
                m_last_heatbeart_ms = bbt::clock::now() + bbt::clock::ms(m_heartbeat_timeout);
        
                if (auto err = N2R_DoHeatBeatReq(); err != std::nullopt)
                    OnError(err.value());            
            }
        }
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

bbt::errcode::ErrOpt ClusterNode::RemoteCall(const std::string& method, bbt::core::Buffer& buffer)
{
    // 找到本地方法

    // 解析协议头

    // 执行方法
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply)
{

    return std::nullopt;
}

void ClusterNode::RequestFromRegistery(bbt::core::Buffer& buffer)
{
    R2NProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.ReadableBytes() < sizeof(R2NProtocolHead)) {
            OnError(bbt::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (R2NProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length) {
            OnError(bbt::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    }

    {
        if (!m_registery_connected && head->protocol_type != R2N_HANDSHAKE_RESP) {
            OnError(bbt::errcode::Errcode("registery not handshake", util::errcode::emErr::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = R2N_Dispatch(head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt) {
        OnError(err.value());
    }
}

void ClusterNode::OnTimeoutFromRegistey(bbt::network::ConnId id)
{
    OnInfo("[ClusterNode] registery lose connect! timeout!");
}

void ClusterNode::OnCloseFromRegistery(bbt::network::ConnId id, const bbt::net::IPAddress& addr)
{
    OnInfo("[ClusterNode] registery lose connect! addr=" + addr.GetIPPort());
    m_registery_connected = false;
    m_registery_client->DelConn();
    _DelayConnectToRegistery();
}

void ClusterNode::RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    OnInfo("recv info");
}

void ClusterNode::OnTimeoutFromNode(bbt::network::ConnId id)
{
    OnInfo("node timeout");
}

void ClusterNode::OnCloseFromNode(bbt::network::ConnId id)
{
    OnInfo("node close");
    m_rpc_client_connid_uuids.erase(id);
    m_rpc_clients.erase(m_rpc_client_connid_uuids[id]);
    m_tcp_server->DelConnect(id);
}

bbt::errcode::ErrOpt ClusterNode::InitNetwork()
{
    if (m_network != nullptr)
        return bbt::errcode::Errcode("already init network!", util::errcode::emErr::CommonErr);

    m_network = std::make_shared<bbt::network::libevent::Network>();
    auto err = m_network->AutoInitThread(2);
    if (err.has_value())
        return bbt::errcode::Errcode("init network failed! " + err->What(), util::errcode::emErr::CommonErr);

    return std::nullopt;
}

void ClusterNode::_DelayConnectToRegistery()
{
    m_connect_to_registery_ms = bbt::clock::nowAfter(bbt::clock::ms{m_reconnect_time});
}

void ClusterNode::_ConnectToRegistery()
{
    if (!(m_registery_client && m_registery_client->GetConn() == nullptr))
        return;

    auto connect_to_registery_err = m_registery_client->AsyncConnect(
    [weak_this{weak_from_this()}]
    (bbt::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn){
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

bbt::errcode::ErrOpt ClusterNode::R2N_Dispatch(emR2NProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return bbt::errcode::Errcode("invalid protocol length type=" + std::to_string(type), util::errcode::emErr::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case R2N_KEEPALIVE_RESP:
        EasyCheck(type, sizeof(R2N_KeepAlive_Resp));
        return R2N_OnHeartBeatResp((R2N_KeepAlive_Resp*)proto);
    case R2N_HANDSHAKE_RESP:
        EasyCheck(type, sizeof(R2N_Handshake_Resp));
        return R2N_OnHandshakeResp((R2N_Handshake_Resp*)proto);
    default:
        return bbt::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

bbt::errcode::ErrOpt ClusterNode::R2N_OnHandshakeResp(R2N_Handshake_Resp* resp)
{
    OnInfo("registery handshake success! msg_code=" + std::to_string(resp->msg_code));
    m_registery_connected = true;
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::R2N_OnHeartBeatResp(R2N_KeepAlive_Resp* resp)
{
    OnDebug("r2n onheartbeat resp!");
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::N2R_DoHandshakeReq()
{
    N2R_Handshake_Req req;
    bbt::core::Buffer buffer;

    req.head.protocol_type = N2R_HANDSHAKE_REQ;
    req.head.protocol_length = sizeof(N2R_Handshake_Req);
    req.head.timestamp_ms = bbt::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return bbt::errcode::Errcode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};

    buffer.WriteString((char*)&req, sizeof(req));

    return SendToRegistery(buffer);
}

bbt::errcode::ErrOpt ClusterNode::N2R_DoHeatBeatReq()
{
    N2R_KeepAlive_Req req;
    bbt::core::Buffer buffer;

    OnDebug("send n2r heartbeat!");
    req.head.protocol_type = N2R_KEEPALIVE_REQ;
    req.head.protocol_length = sizeof(N2R_KeepAlive_Req);
    req.head.timestamp_ms = bbt::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return bbt::errcode::Errcode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};

    buffer.WriteString((char*)&req, sizeof(req));

    return SendToRegistery(buffer);
}

bbt::errcode::ErrOpt ClusterNode::SendToRegistery(bbt::core::Buffer& buffer)
{
    if (m_registery_client == nullptr)
        return bbt::errcode::Errcode{"registery client is null!", util::errcode::CommonErr};

    auto conn = m_registery_client->GetConn();
    if (conn == nullptr)
        return bbt::errcode::Errcode{"registery connection is null!", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.DataSize());
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    auto uuid = m_rpc_client_connid_uuids.find(id);
    if (uuid == m_rpc_client_connid_uuids.end())
        return bbt::errcode::Errcode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};

    auto client = m_rpc_clients.find(uuid->second);
    if (client == nullptr)
        return bbt::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    auto conn = client->second->GetConn();
    if (conn == nullptr)
        return bbt::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.DataSize());
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::N2N_Dispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len)
{
    /**
     * 保证Dispatch时是线程安全的
     */

    
    return std::nullopt;
}

bbt::errcode::ErrOpt ClusterNode::N2N_DoHeartBeat(bbt::network::ConnId id)
{
    N2N_KeepAlive_Req req;
    OnDebug("send n2n heartbeat!");
    auto it = m_rpc_client_connid_uuids.find(id);
    if (it == m_rpc_client_connid_uuids.end())
        return bbt::errcode::Errcode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};
    
    auto conn = m_rpc_clients.find(it->second);
    if (conn == m_rpc_clients.end())
        return bbt::errcode::Errcode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    req.head.protocol_type = N2N_KEEPALIVE_REQ;
    req.head.protocol_length = sizeof(N2N_KeepAlive_Req);
    req.head.timestamp_ms = bbt::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return bbt::errcode::Errcode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};
    
    conn->second->GetConn()->Send((char*)&req, sizeof(req));
    return std::nullopt;
}

}