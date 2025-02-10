#pragma once
#include "engine/ecs/gameobject/GameObject.hpp"
#include "plugin/ecs/npc/NpcDefine.hpp"
#include "plugin/ecs/aoi/AoiObjectComponent.hpp"
#include "plugin/ecs/attribute/Attribute.hpp"
#include "plugin/ecs/buff/BuffList.hpp"

namespace plugin::ecs::npc
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
    engine::ecs::ComponentCSPtr     GetAttributeComp() const;
    engine::ecs::ComponentCSPtr     GetBuffComp() const;
    engine::ecs::ComponentCSPtr     GetAoiObjComp() const;
private:
    NpcId                           m_npc_id{-1};
};

} // namespace share::ecs::npc