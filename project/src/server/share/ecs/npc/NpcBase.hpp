#pragma once
#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/npc/NpcDefine.hpp"
#include "share/ecs/aoi/AoiObjectComponent.hpp"
#include "share/ecs/attribute/Attribute.hpp"
#include "share/ecs/buff/BuffList.hpp"

namespace share::ecs::npc
{

/**
 * 1、属性
 * 2、场景位置
 * 3、buff
 */
class NpcBase:
    public engine::ecs::GameObject
{
public:
    NpcBase();
    ~NpcBase();
    void OnDeath();

private:
    NpcId           m_npc_id{-1};

    attr::Attribute*            m_attr_ctrl{nullptr};    // npc attribute
    buff::BuffList*             m_buff_ctrl{nullptr};    // npc buff
};

} // namespace share::ecs::npc