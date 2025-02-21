#pragma once
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

enum emN2RProtocolType: int8_t
{
    N2R_KEEPALIVE_REQ = 1,
    N2R_PROTOCOL_SIZE,
};

#pragma pack(push, 1)

struct NRProtocolHead
{
    emN2RProtocolType   protocol_type;
    int32_t             protocol_length;
    char                uuid[128];
    int64_t             timestamp_ms;
};

struct N2R_KeepAlive_Req
{
    NRProtocolHead head;
};

#pragma pack(pop)


} // namespace cluster