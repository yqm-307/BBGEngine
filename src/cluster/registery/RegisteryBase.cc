#include <cluster/registery/RegisteryBase.hpp>
#include <cluster/protocol/R2NProtocol.hpp>

namespace cluster
{

RegisteryBase::RegisteryBase()
{
}

util::errcode::ErrOpt RegisteryBase::RecvFromNode(bbt::network::ConnId connid, bbt::core::Buffer& buffer)
{
    NRProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.ReadableBytes() < sizeof(NRProtocolHead))
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    
        head = (NRProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length)
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    }

    // 是否为半连接
    {
        if (m_helf_connect_set.find(connid) != m_helf_connect_set.end())
        {
            if (head->protocol_type != N2R_HANDSHAKE_REQ)
                return util::errcode::ErrCode("half connect not handshake", util::errcode::ErrType::RPC_BAD_PROTOCOL);
            m_helf_connect_set.erase(connid);
        }
    }


    return Dispatch(connid, head->protocol_type, buffer.Peek(), head->protocol_length);
}

RegisterInfo* RegisteryBase::GetNodeRegInfo(std::string uuid)
{
    auto it = m_registery_map.find(uuid);
    if (it == m_registery_map.end())
        return nullptr;
    return &it->second;
}

void RegisteryBase::CloseConn(bbt::network::ConnId connid)
{
    m_helf_connect_set.erase(connid);
    OnNodeLoseConnection(connid);
}

util::errcode::ErrOpt RegisteryBase::Dispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len)
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

util::errcode::ErrOpt RegisteryBase::OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req)
{
    R2N_KeepAlive_Resp resp;

    auto it = m_registery_map.find(req->head.uuid);
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

util::errcode::ErrOpt RegisteryBase::OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req)
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
    
    if (m_registery_map.find(req->head.uuid) != m_registery_map.end())
    {
        SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});
        return util::errcode::ErrCode("node already registed!", util::errcode::ErrType::RPC_BAD_PROTOCOL);
    }
    
    util::other::Uuid uuid;
    uuid.FromByte(req->head.uuid, sizeof(req->head.uuid));
    info.Init(uuid, id);

    m_helf_connect_set.erase(id);
    m_registery_map[req->head.uuid] = info;



    SendToNode(req->head.uuid, bbt::core::Buffer{(char*)&resp, sizeof(resp)});

    return std::nullopt;
}


} // namespace cluster