#include <cluster/registery/Registery.hpp>
#include <cluster/connection/R2NConnection.hpp>
#include <cluster/protocol/R2NProtocol.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <cluster/registery/NodeMgr.hpp>

namespace cluster
{

Registery::Registery():
    m_node_mgr(std::make_shared<NodeMgr>())
{
};

Registery::~Registery()
{
};

void Registery::Init(const bbt::net::IPAddress& listen_addr, int timeout_ms)
{
    m_network = std::make_shared<bbt::network::libevent::Network>();
    m_registery_server = std::make_shared<util::network::TcpServer>(m_network, listen_addr.GetIP(), listen_addr.GetPort(), timeout_ms);

    m_registery_server->Init([weak_this{weak_from_this()}, timeout_ms](auto conn)->std::shared_ptr<util::network::Connection>
    {
        if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
            auto r2n_conn = std::make_shared<R2NConnection>(weak_this, conn, timeout_ms);
            r2n_conn->Init();
            shared_this->OnAccept(r2n_conn->GetConnId());
            return r2n_conn;
        }

        return nullptr;
    });
}

void Registery::Start()
{
    m_registery_server->Start();
}

void Registery::Stop()
{
    m_registery_server->Stop();
}

void Registery::Update()
{
    m_node_mgr->Update();
    // 如果node长时间没有heartbeat，就认为它已经下线，关闭连接

}

NodeRegInfo* Registery::GetNodeRegInfo(const util::other::Uuid& uuid)
{
    return m_node_mgr->GetNodeInfo(uuid);
}

void Registery::CloseConn(bbt::network::ConnId connid)
{
    m_registery_server->ShowDown(connid);
}

NodeState Registery::GetNodeStatus(const util::other::Uuid& uuid) const
{
    return m_node_mgr->GetNodeState(uuid);
}

#pragma region 节点管理

void Registery::RegisterNode(const bbt::net::IPAddress& addr, const util::other::Uuid& uuid)
{
    auto* node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return;

    node_info->SetStatus(NodeState::NODESTATE_ONLINE);
}

void Registery::UnRegisterNode(const util::other::Uuid& uuid)
{
    auto* node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return;

    node_info->SetStatus(NodeState::NODESTATE_OFFLINE);
}

#pragma endregion

bbt::errcode::ErrOpt Registery::SendToNode(const util::other::Uuid& uuid, const bbt::core::Buffer& buffer)
{
    auto* node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return bbt::errcode::Errcode("node not found! uuid=" + uuid.ToString(), util::errcode::emErr::RPC_NOT_FOUND_NODE);

    auto conn = m_registery_server->GetConnectById(node_info->GetConnId());
    if (conn == nullptr)
        return bbt::errcode::Errcode("conn is losed!", util::errcode::emErr::CommonErr);

    conn->Send(buffer.Peek(), buffer.ReadableBytes());
    return std::nullopt;
}

void Registery::OnAccept(bbt::network::ConnId connid)
{
    OnInfo("registery connection accept! connid=" + std::to_string(connid));
    m_half_connect_set.insert(connid);
}

void Registery::OnClose(bbt::network::ConnId connid)
{
    auto node_info = m_node_mgr->GetNodeInfo(connid);
    m_half_connect_set.erase(connid);
    m_registery_server->DelConnect(connid);
    m_node_mgr->NodeOffline(connid);
    OnInfo("registery connection close! connid=" + std::to_string(connid) + "\t uuid=" + node_info->GetUuid().ToString());
}

void Registery::OnTimeout(bbt::network::ConnId connid)
{
    OnInfo("registery connection timeout! connid=" + std::to_string(connid));
}

bool Registery::IsHalfConn(bbt::network::ConnId connid) const
{
    return m_half_connect_set.find(connid) != m_half_connect_set.end();
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
    N2RProtocolHead* head = nullptr;

    auto conn = m_registery_server->GetConnectById(connid);
    if (conn == nullptr) {
        OnError(bbt::errcode::Errcode("connection not found!", util::errcode::emErr::RPC_NOT_FOUND_NODE));
        return;
    }

    {
        if (buffer.ReadableBytes() < sizeof(N2RProtocolHead)) {
            OnError(bbt::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (N2RProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length) {
            OnError(bbt::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }

        // 半连接只能接收握手协议
        if (IsHalfConn(connid) && head->protocol_type != N2R_HANDSHAKE_REQ) {
            OnError(bbt::errcode::Errcode("half-connection can only recv handshake protocol", util::errcode::emErr::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = N2RDispatch(connid, head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt)
        OnError(err.value());
}

bbt::errcode::ErrOpt Registery::N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return bbt::errcode::Errcode("invalid protocol length type=" + std::to_string(type), util::errcode::emErr::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case N2R_KEEPALIVE_REQ:
        EasyCheck(type, sizeof(N2R_KeepAlive_Req));
        return OnHeartBeat(id, (N2R_KeepAlive_Req*)proto);
    case N2R_HANDSHAKE_REQ:
        EasyCheck(type, sizeof(N2R_Handshake_Req));
        return OnHandshake(id, (N2R_Handshake_Req*)proto);
    default:
        return bbt::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

bbt::errcode::ErrOpt Registery::OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req)
{
    R2N_KeepAlive_Resp resp;
    util::other::Uuid uuid{req->head.uuid, sizeof(req->head.uuid)};

    OnDebug("on heartbeat!");
    // auto it = m_registery_map.find(util::other::Uuid{req->head.uuid, sizeof(req->head.uuid)});
    if (GetNodeStatus(uuid) != NodeState::NODESTATE_ONLINE)
        return bbt::errcode::Errcode("[OnHeartBeat] node not registed!", util::errcode::emErr::RPC_NOT_FOUND_NODE);

    resp.head.protocol_type = R2N_KEEPALIVE_RESP;
    resp.head.protocol_length = sizeof(R2N_KeepAlive_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::clock::gettime();
    
    SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
    return std::nullopt;
}

bbt::errcode::ErrOpt Registery::OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req)
{
    R2N_Handshake_Resp resp;
    NodeRegInfo info;
    util::other::Uuid uuid{req->head.uuid, sizeof(req->head.uuid)};
    
    resp.head.protocol_type = R2N_HANDSHAKE_RESP;
    resp.head.protocol_length = sizeof(R2N_Handshake_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::clock::gettime();
    resp.msg_code = 0;

    if (m_half_connect_set.find(id) == m_half_connect_set.end()) {
        SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return bbt::errcode::Errcode("not a helf-connection!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    }
    
    if (GetNodeStatus(uuid) == NodeState::NODESTATE_ONLINE)
    {
        SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return bbt::errcode::Errcode("node already registed!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    }
    
    info.Init(uuid, bbt::net::IPAddress{req->node_ip, req->node_port});
    info.SetConnId(id);
    m_half_connect_set.erase(id);
    m_node_mgr->RegisterNode(uuid, info);

    SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});

    OnInfo("[Registery] handshake success! uuid=" + uuid.ToString());
    return std::nullopt;
}

}