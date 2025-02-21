#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/clock/ClockSystem.hpp"

namespace plugin::ecs::clock
{

int64_t ClockSystem::GetFrame(EntitySPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return -1;
    return comp->GetFrame();
}

bbt::clock::Timestamp<> ClockSystem::GetFrameTime(EntitySPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return bbt::clock::Timestamp<>();
    return comp->GetFrameTime();
}

std::shared_ptr<ClockComp> ClockSystem::GetComponent(EntitySPtr gameobject)
{
    return gameobject->GetComponent<ClockComp>();
}

}