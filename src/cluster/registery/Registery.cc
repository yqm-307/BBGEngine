#include <cluster/registery/Registery.hpp>
#include <cluster/connection/R2NConnection.hpp>
#include <cluster/protocol/R2NProtocol.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <cluster/registery/NodeMgr.hpp>

#define BBGENGINE_MODULE_NAME "[BBG::Registery]"

namespace cluster
{

Registery::Registery():
    m_node_mgr(std::make_shared<NodeMgr>())
{
};

Registery::~Registery()
{
};

void Registery::Init(const util::network::IPAddress& listen_addr, int timeout_ms)
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

NodeRegInfo::SPtr Registery::GetNodeRegInfo(const util::other::Uuid& uuid)
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

void Registery::RegisterNode(const util::network::IPAddress& addr, const util::other::Uuid& uuid)
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

util::errcode::ErrOpt Registery::SendToNode(const util::other::Uuid& uuid, const bbt::core::Buffer& buffer)
{
    auto node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return util::errcode::Errcode("node not found! uuid=" + uuid.ToString(), util::errcode::emErr::RPC_NOT_FOUND_NODE);

    auto conn = m_registery_server->GetConnectById(node_info->GetConnId());
    if (conn == nullptr)
        return util::errcode::Errcode("conn is losed!", util::errcode::emErr::CommonErr);

    conn->Send(buffer.Peek(), buffer.Size());
    return std::nullopt;
}

void Registery::OnSendToNode(util::errcode::ErrOpt err, size_t len)
{
    if (err != std::nullopt)
        OnError(err.value());
    else
        OnInfo(BBGENGINE_MODULE_NAME "send to registery success! len=" + std::to_string(len));
}

void Registery::OnAccept(bbt::network::ConnId connid)
{
    OnInfo("registery connection accept! connid=" + std::to_string(connid));
    m_half_connect_set.insert(connid);
}

void Registery::OnClose(bbt::network::ConnId connid)
{
    util::other::Uuid uuid;
    m_half_connect_set.erase(connid);
    m_registery_server->DelConnect(connid);
    
    auto node_info = m_node_mgr->GetNodeInfo(connid);
    if (node_info != nullptr) {
        m_node_mgr->NodeOffline(connid);
        uuid = node_info->GetUuid();
        OnInfo(BBGENGINE_MODULE_NAME "node offline! uuid=" + uuid.ToString());
    }

    OnInfo("registery connection close! connid=" + std::to_string(connid));
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
        OnError(util::errcode::Errcode("connection not found!", util::errcode::emErr::RPC_NOT_FOUND_NODE));
        return;
    }

    {
        if (buffer.Size() < sizeof(N2RProtocolHead)) {
            OnError(util::errcode::Errcode("buffer not enough", util::errcode::emErr::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (N2RProtocolHead*)buffer.Peek();
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

    if (auto err = N2RDispatch(connid, head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt)
        OnError(err.value());
}

util::errcode::ErrOpt Registery::N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return util::errcode::Errcode("invalid protocol length type=" + std::to_string(type), util::errcode::emErr::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case N2R_KEEPALIVE_REQ:
        EasyCheck(type, sizeof(N2R_KeepAlive_Req));
        return OnHeartBeat(id, (N2R_KeepAlive_Req*)proto);
    case N2R_HANDSHAKE_REQ:
        EasyCheck(type, sizeof(N2R_Handshake_Req));
        return OnHandshake(id, (N2R_Handshake_Req*)proto);
    case N2R_REGISTER_METHOD_REQ:
        EasyCheck(type, sizeof(N2R_RegisterMethod_Req));
        return OnRegisterMethod(id, (N2R_RegisterMethod_Req*)proto);
    case N2R_GET_NODES_INFO_REQ:
        EasyCheck(type, sizeof(N2R_GetNodesInfo_Req));
        return OnGetNodesInfo(id, (N2R_GetNodesInfo_Req*)proto);
    default:
        return util::errcode::Errcode("unknown protocol type=" + std::to_string(type), util::errcode::emErr::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

util::errcode::ErrOpt Registery::OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req)
{
    R2N_KeepAlive_Resp resp;
    util::other::Uuid uuid{req->head.uuid, sizeof(req->head.uuid)};
    auto node_info = m_node_mgr->GetNodeInfo(uuid);

    OnDebug("on heartbeat! ts=" + std::to_string(req->head.timestamp_ms) + " uuid=" + uuid.ToString());
    if (node_info == nullptr)
        return util::errcode::Errcode("[OnHeartBeat] node not registed!", util::errcode::emErr::RPC_NOT_FOUND_NODE);

    resp.head.protocol_type = R2N_KEEPALIVE_RESP;
    resp.head.protocol_length = sizeof(R2N_KeepAlive_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::core::clock::gettime();

    node_info->OnHeartBeat();
    
    SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
    return std::nullopt;
}

util::errcode::ErrOpt Registery::OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req)
{
    R2N_Handshake_Resp resp;
    auto info = std::make_shared<NodeRegInfo>();
    util::other::Uuid uuid{req->head.uuid, sizeof(req->head.uuid)};
    
    resp.head.protocol_type = R2N_HANDSHAKE_RESP;
    resp.head.protocol_length = sizeof(R2N_Handshake_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::core::clock::gettime();
    resp.msg_code = 0;

    if (m_half_connect_set.find(id) == m_half_connect_set.end()) {
        SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return util::errcode::Errcode("not a helf-connection!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    }
    
    if (GetNodeStatus(uuid) == NodeState::NODESTATE_ONLINE)
    {
        SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return util::errcode::Errcode("node already registed!", util::errcode::emErr::RPC_BAD_PROTOCOL);
    }
    
    info->Init(uuid, util::network::IPAddress{req->node_ip, req->node_port});
    info->SetConnId(id);
    info->SetStatus(NodeState::NODESTATE_ONLINE);
    m_half_connect_set.erase(id);
    m_node_mgr->RegisterNode(uuid, info);

    SendToNode(uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});

    OnInfo("[Registery] handshake success! uuid=" + uuid.ToString());
    return std::nullopt;
}

util::errcode::ErrOpt Registery::OnRegisterMethod(bbt::network::ConnId id, N2R_RegisterMethod_Req* req)
{
    R2N_RegisterMethod_Resp resp;
    resp.head.protocol_type = R2N_REGISTER_METHOD_RESP;
    resp.head.protocol_length = sizeof(R2N_RegisterMethod_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::core::clock::gettime();

    auto node_info = m_node_mgr->GetNodeInfo(id);
    if (node_info == nullptr) {
        resp.msg_code = emRegisterMethodErr::Failed;
        m_registery_server->Send(id, (char*)&resp, sizeof(resp));
        return util::errcode::Errcode("node not found!", util::errcode::emErr::RPC_NOT_FOUND_NODE);
    }


    node_info->AddMethod(req->method_name);
    resp.msg_code = emRegisterMethodErr::Succ;
    m_registery_server->Send(id, (char*)&resp, sizeof(resp));
    return std::nullopt;
}

util::errcode::ErrOpt Registery::OnGetNodesInfo(bbt::network::ConnId id, N2R_GetNodesInfo_Req* req)
{
    R2N_GetNodesInfo_Resp resp;
    R2N_GetNodesInfo_Resp::NodeInfo info;
    bbt::core::Buffer buffer;
    
    resp.head.protocol_type = R2N_GET_NODES_INFO_RESP;
    resp.head.protocol_length = sizeof(R2N_GetNodesInfo_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::core::clock::gettime();
    
    auto nodes = m_node_mgr->GetAllNodeInfo();
    resp.node_count = nodes.size();
    buffer.Write(resp);
    for (auto&& node : nodes) {
        info.port = node->GetNodeAddr().GetPort();
        node->GetUuid().ToByte(info.uuid, sizeof(info.uuid));
        memcpy(info.ip, node->GetNodeAddr().GetIP().c_str(), sizeof(info.ip));
        buffer.Write(info);
    }

    m_registery_server->Send(id, (char*)&resp, sizeof(resp));
    return std::nullopt;
}


}

#undef BBGENGINE_MODULE_NAME