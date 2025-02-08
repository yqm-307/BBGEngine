#include "plugin/ecs/npc/NpcBase.hpp"


namespace share::ecs::npc
{

NpcBase::NpcBase():
    GameObject(share::ecs::emEntityType::EM_ENTITY_TYPE_NPC)
{
    auto attr_component         = G_ComponentMgr()->Create<attr::Attribute>();
    auto bufflist_component     = G_ComponentMgr()->Create<buff::BuffList>();
    auto aoi_object_component   = G_ComponentMgr()->Create<ecs::aoi::AoiObjectComponent>(ecs::aoi::AoiEntityFlag::Watcher);


    AssertWithInfo(AddComponent(attr_component),        "npc component mount failed!");
    AssertWithInfo(AddComponent(bufflist_component),    "npc component mount failed!");
    AssertWithInfo(AddComponent(aoi_object_component),  "npc component mount failed!");
}

NpcBase::~NpcBase()
{
}

engine::ecs::ComponentCSPtr NpcBase::GetAttributeComp() const
{
    return GetComponent(ecs::EM_COMPONENT_TYPE_ATTRIBUTE);
}

engine::ecs::ComponentCSPtr NpcBase::GetBuffComp() const
{
    return GetComponent(ecs::EM_COMPONENT_TYPE_BUFFLIST);
}

engine::ecs::ComponentCSPtr NpcBase::GetAoiObjComp() const
{
    return GetComponent(ecs::EM_COMPONENT_TYPE_AOI_OBJECT);
}


}