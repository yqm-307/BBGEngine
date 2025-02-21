#include <cluster/registery/RegisteryBase.hpp>
#include <cluster/protocol/R2NProtocol.hpp>

namespace cluster
{

RegisteryBase::RegisteryBase()
{
}

util::errcode::ErrOpt RegisteryBase::RecvFromNode(bbt::core::Buffer& buffer)
{
    NRProtocolHead* head = nullptr;

    if (buffer.ReadableBytes() < sizeof(NRProtocolHead))
        return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);

    head = (NRProtocolHead*)buffer.Peek();
    if (buffer.ReadableBytes() < head->protocol_length)
        return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);

    return Dispatch(head->protocol_type, buffer.Peek(), head->protocol_length);
}

util::errcode::ErrOpt RegisteryBase::Dispatch(emN2RProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return util::errcode::ErrCode("invalid protocol length type=" + std::to_string(type), util::errcode::ErrType::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case N2R_KEEPALIVE_REQ:
        EasyCheck(type, sizeof(N2R_KeepAlive_Req));
        return OnHeartBeat((N2R_KeepAlive_Req*)proto);
    default:
        return util::errcode::ErrCode("unknown protocol type=" + std::to_string(type), util::errcode::ErrType::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

util::errcode::ErrOpt RegisteryBase::OnHeartBeat(N2R_KeepAlive_Req* req)
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



} // namespace cluster