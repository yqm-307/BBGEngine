#pragma once
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

enum emN2RProtocolType: int8_t
{
    N2R_KEEPALIVE_REQ = 1,
    N2R_HANDSHAKE_REQ,
    N2R_PROTOCOL_SIZE,
};

#pragma pack(push, 1)

struct N2RProtocolHead
{
    emN2RProtocolType   protocol_type;
    int32_t             protocol_length;
    char                uuid[128];
    int64_t             timestamp_ms;
};

struct N2R_KeepAlive_Req
{
    N2RProtocolHead head;
};

struct N2R_Handshake_Req
{
    N2RProtocolHead head;
};

#pragma pack(pop)


} // namespace cluster