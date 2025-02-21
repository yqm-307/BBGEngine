#include <cluster/registery/RegisterInfo.hpp>

namespace cluster
{

RegisterInfo::RegisterInfo()
{
}

RegisterInfo::~RegisterInfo()
{
}

void RegisterInfo::Clear()
{
    m_state = NodeState::NODESTATE_DEFAULT;
    m_method_info_map.clear();
    m_last_active_time = bbt::clock::Timestamp<bbt::clock::ms>();
    m_heart_beat_interval = 0;
}

void RegisterInfo::Init(util::other::Uuid uuid)
{
    m_uuid = uuid;
    m_state = NodeState::NODESTATE_ONLINE;
    m_last_active_time = bbt::clock::Timestamp<bbt::clock::ms>();

    // 默认心跳间隔
    m_heart_beat_interval = 1000;
}

void RegisterInfo::AddMethod(const std::string& method_name)
{
    m_method_info_map.insert(method_name);
}

void RegisterInfo::DelMethod(const std::string& method_name)
{
    m_method_info_map.erase(method_name);

    // 如果没有方法了，就下线
    if (m_method_info_map.empty())
    {
        m_state = NodeState::NODESTATE_OFFLINE;
    }
}

bool RegisterInfo::HasMethod(const std::string& method_name) const
{
    return m_method_info_map.find(method_name) != m_method_info_map.end();
}

void RegisterInfo::Active()
{
    m_last_active_time = bbt::clock::now();
}

} // namespace cluster