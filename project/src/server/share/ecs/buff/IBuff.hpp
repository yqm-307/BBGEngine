#pragma once
#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/npc/NpcDefine.hpp"
#include "share/ecs/buff/BuffDefine.hpp"

namespace share::ecs::buff
{

template<class NpcPtr, class BuffPtr>
class IBuff
{
public:
    typedef NpcPtr Object;

    // 当前目标npc造成伤害
    virtual void OnDoDamage(Object injured, uint64_t damage_value) = 0;
    // 当前目标npc受到伤害
    virtual void OnBeDamage(Object hurter, uint64_t damage_value) = 0;
    // 当前目标npc造成回复生命
    virtual void OnBeRegenerate(Object healer, uint64_t value) = 0;
    // 当前buff超时
    virtual void OnTimeout() = 0;
    // 当前npc添加buff
    virtual void OnAddBuff(BuffPtr buff) = 0;
    // 当前npc移除buff
    virtual void OnDelBuff(BuffPtr buff) = 0;
    // 当前npc状态变化
    virtual void OnChangeState() = 0;

    // 初始npc造成伤害
    virtual void OnMasterDoDamage(Object raw_master, Object injured, uint64_t damage_value) = 0;
    // 初始npc受到伤害
    virtual void OnMasterBeDamage(Object hurter, Object raw_master, uint64_t damage_value) = 0;
    // 初始npc回复生命
    virtual void OnMasterBeRegenerate(Object healer, uint64_t value) = 0;
    // 初始npc buff超时
    virtual void OnMasterTimeout() = 0;
    // 初始npc添加buff
    virtual void OnMasterAddBuff(Object master, BuffPtr buff) = 0;
    // 初始npc移除buff
    virtual void OnMasterDelBuff(Object master, BuffPtr buff) = 0;
    // 初始npc状态变化
    virtual void OnMasterChangeState() = 0;

};

} // namespace share::ecs::npc
