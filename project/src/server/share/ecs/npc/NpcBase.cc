#include "share/ecs/npc/NpcBase.hpp"


namespace share::ecs::npc
{

NpcBase::NpcBase():
    GameObject(share::ecs::emEntityType::EM_ENTITY_TYPE_NPC)
{
    auto attr_component         = G_ComponentMgr()->Create<Attribute>();
    auto bufflist_component     = G_ComponentMgr()->Create<BuffList>();


    AssertWithInfo(AddComponent(attr_component),        "component mount failed!");
    AssertWithInfo(AddComponent(bufflist_component),    "component mount failed!");
}

NpcBase::~NpcBase()
{
    DelComponent();
}


}