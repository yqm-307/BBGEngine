#pragma once
#include "share/ecs/buff/IBuff.hpp"

namespace share::ecs::buff
{

class BuffBase:
    public IBuff<engine::ecs::GameObjectSPtr, std::shared_ptr<BuffBase>>
{
public:
    typedef std::shared_ptr<BuffBase> SPtr;
    virtual BuffTriggerType     GetType() = 0;
    virtual BuffId              GetBuffId() = 0;
    npc::NpcId                  GetTargetId() { return m_cur_target_npc; }
    npc::NpcId                  GetMasterId() { return m_raw_master_npc; }

    virtual void                OnUpdate() = 0;
    virtual void                OnEvent(
        engine::ecs::GameObjectSPtr from,
        engine::ecs::GameObjectSPtr to,
        BuffTriggerType type,
        const void* param) = 0;

protected:
    // 当前目标npc造成伤害
    virtual void OnDoDamage(Object injured, uint64_t damage_value) {};
    // 当前目标npc受到伤害
    virtual void OnBeDamage(Object hurter, uint64_t damage_value) {};
    // 当前目标npc造成回复生命
    virtual void OnBeRegenerate(Object healer, uint64_t value) {};
    // 当前buff超时
    virtual void OnTimeout() {};
    // 当前npc添加buff
    virtual void OnAddBuff(BuffPtr buff) {};
    // 当前npc移除buff
    virtual void OnDelBuff(BuffPtr buff) {};
    // 当前npc状态变化
    virtual void OnChangeState() {};

    // 初始npc造成伤害
    virtual void OnMasterDoDamage(Object raw_master, Object injured, uint64_t damage_value) {};
    // 初始npc受到伤害
    virtual void OnMasterBeDamage(Object hurter, Object raw_master, uint64_t damage_value) {};
    // 初始npc回复生命
    virtual void OnMasterBeRegenerate(Object healer, uint64_t value) {};
    // 初始npc buff超时
    virtual void OnMasterTimeout() {};
    // 初始npc添加buff
    virtual void OnMasterAddBuff(Object master, BuffPtr buff) {};
    // 初始npc移除buff
    virtual void OnMasterDelBuff(Object master, BuffPtr buff) {};
    // 初始npc状态变化
    virtual void OnMasterChangeState() {};
protected:
    int                         m_life_frame{0};                    // buff 生命期（帧数）
    npc::NpcId                  m_cur_target_npc{-1};               // 当前的buff目标（受影响者）
    /**
     *  原则上buff的原始主人不可以变动。
     * 
     *  如果A是初始释放着，最后通过N个玩家蔓延到了
     *  C身上，在C触发了此buff，那么有任何buff效果
     *  都需要作用到A身上。但是目前游戏技能的设计很
     *  少见这样的规则。
     *
     *  解决方案：
     *  1、无归属buff，没有归属npc，自然作用不到原始npc
     *  2、脱手buff，后续任何效果跳过master对象 
     */
    npc::NpcId                  m_raw_master_npc{-1};               // buff的原始主人（释放者）
    
};

} // namespace share::ecs::npc
