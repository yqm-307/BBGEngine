#pragma once
#include "share/ecs/buff/IBuff.hpp"

namespace share::ecs::buff
{

class BuffBase:
    public IBuff<engine::ecs::GameObjectSPtr, std::shared_ptr<BuffBase>>
{
public:

    virtual BuffType GetType() = 0;

    virtual BuffId GetBuffId() = 0;
    npc::NpcId GetTargetId() { return m_cur_target_npc; }
    npc::NpcId GetMasterId() { return m_raw_master_npc; }
protected:
    npc::NpcId               m_cur_target_npc{-1};               // 当前的buff目标（受影响者）
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
    npc::NpcId               m_raw_master_npc{-1};               // buff的原始主人（释放者）
    
};

} // namespace share::ecs::npc
