#pragma once
#include <cluster/rpc/RpcSerializer.hpp>
#include <cluster/ClusterDefine.hpp>

namespace cluster
{

enum emR2NProtocolType: int8_t
{
    R2N_PROTOCOL_NONE = 0,
    R2N_KEEPALIVE_RESP = 1,
    R2N_HANDSHAKE_RESP,
    R2N_REGISTER_METHOD_RESP,
    R2N_GET_NODES_INFO_RESP,
    R2N_PROTOCOL_SIZE,
};

#pragma pack(push, 1)

struct R2NProtocolHead
{
    emR2NProtocolType   protocol_type;
    int32_t             protocol_length;
    char                uuid[16];
    int64_t             timestamp_ms;
};

struct R2N_KeepAlive_Resp
{
    R2NProtocolHead head;
};

struct R2N_Handshake_Resp
{
    R2NProtocolHead head;
    int             msg_code;
};

struct R2N_RegisterMethod_Resp
{
    R2NProtocolHead head;
    emRegisterMethodErr msg_code;
};

struct R2N_GetNodesInfo_Resp
{
    struct NodeInfo
    {
        char            uuid[16];
        char            ip[16];
        short           port;
    };

    R2NProtocolHead head;
    int             node_count;
    NodeInfo        nodes[0];
};

#pragma pack(pop)

} // namespace cluster