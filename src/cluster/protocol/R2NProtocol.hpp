#pragma once
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

enum emR2NProtocolType: int8_t
{
    R2N_KEEPALIVE_RESP = 1,
    R2N_PROTOCOL_SIZE,
};

#pragma pack(push, 1)

struct R2NProtocolHead
{
    emR2NProtocolType   protocol_type;
    int32_t             protocol_length;
    char                uuid[128];
    int64_t             timestamp_ms;
};

struct R2N_KeepAlive_Resp
{
    R2NProtocolHead head;
};

#pragma pack(pop)

} // namespace cluster