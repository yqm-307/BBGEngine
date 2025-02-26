#include <cluster/registery/RegisteryServer.hpp>
#include <cluster/registery/Registery.hpp>
#include <cluster/protocol/R2NProtocol.hpp>
#include <cluster/protocol/N2RProtocol.hpp>

namespace cluster
{

Registery::Registery()
{
};

Registery::~Registery()
{
};

void Registery::Init(const bbt::net::IPAddress& listen_addr, int timeout_ms)
{
    m_network = std::make_shared<bbt::network::libevent::Network>();
    // new RegisteryServer(m_network, shared_from_this(), listen_addr.GetIP(), listen_addr.GetPort(), timeout_ms);
    m_server = std::make_shared<RegisteryServer>(m_network, shared_from_this(), listen_addr.GetIP(), listen_addr.GetPort(), timeout_ms);
    m_server->Init();
}

void Registery::Start()
{
    m_server->Start();
}

void Registery::Stop()
{
    m_server->Stop();
}

RegisterInfo* Registery::GetNodeRegInfo(std::string uuid)
{
    util::other::Uuid uuid_obj;
    uuid_obj.FromByte(uuid.c_str(), uuid.size());
    auto it = m_registery_map.find(uuid_obj);
    if (it == m_registery_map.end())
        return nullptr;
    return &it->second;
}

void Registery::CloseConn(bbt::network::ConnId connid)
{
    m_server->ShowDown(connid);
}


util::errcode::ErrOpt Registery::SendToNode(const char* uuid, const bbt::core::Buffer& buffer)
{
    auto* node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return util::errcode::ErrCode("node not found! uuid=" + std::string{uuid}, util::errcode::ErrType::RPC_NOT_FOUND_NODE);

    auto conn = m_server->GetConnectById(node_info->GetConnId());
    if (conn == nullptr)
        return util::errcode::ErrCode("conn is losed!", util::errcode::ErrType::CommonErr);

    conn->Send(buffer.Peek(), buffer.ReadableBytes());
    return std::nullopt;
}

void Registery::OnAccept(bbt::network::ConnId connid)
{
    m_helf_connect_set.insert(connid);
}

void Registery::OnClose(bbt::network::ConnId connid)
{
    auto uuid = m_connid_uuid_map[connid];
    m_connid_uuid_map.erase(connid);
    m_helf_connect_set.erase(connid);
    m_server->DelConnect(connid);
    OnInfo("registery connection close! connid=" + std::to_string(connid) + "\t uuid=" + uuid.ToString());
}

void Registery::OnTimeout(bbt::network::ConnId connid)
{
    OnInfo("registery connection timeout! connid=" + std::to_string(connid));
}


util::errcode::ErrOpt Registery::RecvFromNode(bbt::network::ConnId connid, bbt::core::Buffer& buffer)
{
    N2RProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.ReadableBytes() < sizeof(N2RProtocolHead))
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    
        head = (N2RProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length)
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    }

    return N2RDispatch(connid, head->protocol_type, buffer.Peek(), head->protocol_length);
}

util::errcode::ErrOpt Registery::N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return util::errcode::ErrCode("invalid protocol length type=" + std::to_string(type), util::errcode::ErrType::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case N2R_KEEPALIVE_REQ:
        EasyCheck(type, sizeof(N2R_KeepAlive_Req));
        return OnHeartBeat(id, (N2R_KeepAlive_Req*)proto);
    case N2R_HANDSHAKE_REQ:
        EasyCheck(type, sizeof(N2R_Handshake_Req));
        return OnHandshake(id, (N2R_Handshake_Req*)proto);
    default:
        return util::errcode::ErrCode("unknown protocol type=" + std::to_string(type), util::errcode::ErrType::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

util::errcode::ErrOpt Registery::OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req)
{
    R2N_KeepAlive_Resp resp;

    auto it = m_registery_map.find(util::other::Uuid{req->head.uuid, sizeof(req->head.uuid)});
    if (it == m_registery_map.end())
        return util::errcode::ErrCode("node not registed!", util::errcode::ErrType::RPC_NOT_FOUND_NODE);

    it->second.Active();

    resp.head.protocol_type = R2N_KEEPALIVE_RESP;
    resp.head.protocol_length = sizeof(R2N_KeepAlive_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::clock::gettime();
    
    SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
    return std::nullopt;
}

util::errcode::ErrOpt Registery::OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req)
{
    R2N_Handshake_Resp resp;
    RegisterInfo info;
    
    resp.head.protocol_type = R2N_HANDSHAKE_RESP;
    resp.head.protocol_length = sizeof(R2N_Handshake_Resp);
    memcpy(resp.head.uuid, req->head.uuid, sizeof(req->head.uuid));
    resp.head.timestamp_ms = bbt::clock::gettime();
    resp.msg_code = 0;

    if (m_helf_connect_set.find(id) == m_helf_connect_set.end()) {
        SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return util::errcode::ErrCode("not a helf-connection!", util::errcode::ErrType::RPC_BAD_PROTOCOL);
    }
    
    if (m_registery_map.find(util::other::Uuid{req->head.uuid, sizeof(req->head.uuid)}) != m_registery_map.end())
    {
        SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return util::errcode::ErrCode("node already registed!", util::errcode::ErrType::RPC_BAD_PROTOCOL);
    }
    
    util::other::Uuid uuid;
    uuid.FromByte(req->head.uuid, sizeof(req->head.uuid));
    info.Init(uuid, id);

    m_connid_uuid_map[id] = uuid;
    m_helf_connect_set.erase(id);
    m_registery_map[uuid] = info;

    SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});

    OnInfo("[ClusterNode] handshake success! uuid=" + uuid.ToString());
    return std::nullopt;
}

}