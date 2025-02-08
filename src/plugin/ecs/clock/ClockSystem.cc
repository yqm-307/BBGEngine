#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/clock/ClockSystem.hpp"

namespace share::ecs::clock
{

int64_t ClockSystem::GetFrame(GameObjectSPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return -1;
    return comp->GetFrame();
}

bbt::clock::Timestamp<> ClockSystem::GetFrameTime(GameObjectSPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return bbt::clock::Timestamp<>();
    return comp->GetFrameTime();
}

std::shared_ptr<ClockComp> ClockSystem::GetComponent(GameObjectSPtr gameobject)
{
    auto comp = gameobject->GetComponent(EM_COMPONENT_TYPE_CLOCK_MODULE);
    if (comp == nullptr)
        return nullptr;
    
    auto clock_comp = std::dynamic_pointer_cast<ClockComp>(comp);
    return clock_comp;
}

}