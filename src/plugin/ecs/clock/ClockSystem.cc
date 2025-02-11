#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/clock/ClockSystem.hpp"

namespace plugin::ecs::clock
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
    return gameobject->GetComponent<ClockComp>();
}

}