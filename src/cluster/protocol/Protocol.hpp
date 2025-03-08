#pragma once
#include <cluster/protocol/Protocol.pb.h>

namespace cluster::protocol
{

constexpr uint32_t MAX_PROTOCOL_LENGTH = 2 * 1024 * 1024;
constexpr uint32_t METHOD_NAME_MAX_LEN = 128;

#pragma pack(push, 1)

struct ProtocolHead
{
    int                 protocol_type;
    uint32_t            protocol_length;
};

#pragma pack(pop)

} // namespace cluster