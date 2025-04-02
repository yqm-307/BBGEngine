#pragma once
#include <util/errcode/ErrCodeDef.hpp>
#include <bbt/network/TcpServer.hpp>

namespace cluster
{
class BufferMgr
{
public:
    BufferMgr() = default;
    ~BufferMgr() = default;

    util::errcode::ErrOpt AddBuffer(bbt::network::ConnId connid, const bbt::core::Buffer& buffer);

    void RemoveBuffer(bbt::network::ConnId connid);

    util::errcode::ErrOpt DoBuffer(bbt::network::ConnId connid, std::function<void(bbt::core::Buffer&)> func);

private:
    std::unordered_map<bbt::network::ConnId, bbt::core::Buffer> m_buffer_map;
    std::mutex m_buffer_map_lock;
};
}