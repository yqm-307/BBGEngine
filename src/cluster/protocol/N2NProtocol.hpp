#pragma once
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

enum emN2NProtocolType: int8_t
{
    N2N_KEEPALIVE_REQ = 1,
    N2N_PROTOCOL_SIZE,
};

#pragma pack(push, 1)

struct N2NProtocolHead
{
    emN2NProtocolType   protocol_type;
    int32_t             protocol_length;
    char                uuid[128];
    int64_t             timestamp_ms;
};

struct N2N_KeepAlive_Req
{
    N2NProtocolHead     head;
};

#pragma pack(pop)

} // namespace cluster