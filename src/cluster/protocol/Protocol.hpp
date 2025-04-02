#pragma once
#include <cluster/ClusterDefine.hpp>
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

class Helper
{
public:
    /**
     * @brief 协议解析，从buffer中解析出完整的协议，并push到protocols中，解析成功的数据会从buffer中删除
     * 
     * @param buffer 待解析数据
     * @param protocols 解析后的协议
     * @return util::errcode::ErrOpt 
     */
    static util::errcode::ErrOpt ParseProtocolFromBuffer(bbt::core::Buffer& buffer, std::vector<bbt::core::Buffer>& protocols);

};

} // namespace cluster