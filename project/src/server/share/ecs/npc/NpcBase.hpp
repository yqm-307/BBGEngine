#pragma once
#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/npc/NpcDefine.hpp"
#include "share/ecs/aoi/AoiObjectComponent.hpp"
#include "share/ecs/npc/Attribute.hpp"
#include "share/ecs/npc/BuffList.hpp"

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
    
};

} // namespace share::ecs::npc