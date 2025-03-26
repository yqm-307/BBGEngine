#include <cluster/rpc/BufferMgr.hpp>

namespace cluster
{

using namespace bbt::core::errcode;

ErrOpt BufferMgr::AddBuffer(bbt::network::ConnId connid, const bbt::core::Buffer& buffer)
{
    std::lock_guard<std::mutex> lock(m_buffer_map_lock);
    if (m_buffer_map.find(connid) != m_buffer_map.end())
        return util::errcode::Errcode("Buffer already exists", util::errcode::emErr::CommonErr);

    m_buffer_map[connid] = buffer;
    return std::nullopt;
}

void BufferMgr::RemoveBuffer(bbt::network::ConnId connid)
{
    std::lock_guard<std::mutex> lock(m_buffer_map_lock);
    m_buffer_map.erase(connid);
}

util::errcode::ErrOpt BufferMgr::DoBuffer(bbt::network::ConnId connid, std::function<void(bbt::core::Buffer&)> func)
{
    std::lock_guard<std::mutex> lock(m_buffer_map_lock);
    auto it = m_buffer_map.find(connid);
    if (it == m_buffer_map.end())
        return util::errcode::Errcode("Buffer not found", util::errcode::emErr::CommonErr);

    func(it->second);        
    return std::nullopt;
}


}