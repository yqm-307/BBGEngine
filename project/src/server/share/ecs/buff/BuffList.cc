#include "share/ecs/buff/BuffList.hpp"

namespace share::ecs::buff
{

void BuffList::OnUpdate()
{
    for (auto buff : m_buff_map) {
        buff.second->OnUpdate();
    }
}

void BuffList::OnEvent(const OnEventParams& params)
{
    if (params.from == nullptr || params.to == nullptr)
        return;

    switch (params.trigger_type)
    {
    case BuffTriggerType::ON_REAL_HP_CHANGE:
        OnCommonEvent(params.from, params.to, BuffTriggerType::ON_REAL_HP_CHANGE, static_cast<const OnRealHpChangeParams*>(params.param));
        break;
    default:
        break;
    }
}

void BuffList::OnCommonEvent(
    engine::ecs::GameObjectSPtr from,
    engine::ecs::GameObjectSPtr to,
    BuffTriggerType type,
    const void* param)
{
    if (param == nullptr)
        return;
    
    auto it = m_event_map.find(type);
    if (it == m_event_map.end())
        return;
    
    for (auto buff : it->second) {
        buff.second->OnEvent(from, to, type, param);
    }
}

bool BuffList::AddBuff(BuffBase::SPtr buff)
{
    BuffTriggerType type;

    // 合法性检测
    if (buff == nullptr)
        return false;
    // 事件注册
    type = buff->GetType();
    auto it = m_event_map.find(type);
    if (it == m_event_map.end())
        m_event_map.insert(std::make_pair(type, std::unordered_map<BuffId, BuffBase::SPtr>()));

    // 加入数据集中
    auto [_, success] = m_event_map[type].insert(std::make_pair(buff->GetBuffId(), buff));

    return success;
}

bool BuffList::DelBuff(BuffId id)
{
    BuffTriggerType type;
    BuffBase::SPtr  buff = nullptr;

    buff = GetBuffById(id);
    if (buff == nullptr)
        return false; // buff不存在

    type = buff->GetType();
    auto it = m_event_map.find(type);
    if (it == m_event_map.end())
        return false;

    size_t del_num = it->second.erase(id);
    
    return del_num > 0;
}

BuffBase::SPtr BuffList::GetBuffById(BuffId id)
{
    auto it = m_buff_map.find(id);
    return (it == m_buff_map.end() ? nullptr : it->second) ;
}


}