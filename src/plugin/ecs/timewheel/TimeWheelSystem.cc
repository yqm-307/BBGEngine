#include <engine/ecs/gameobject/GameObject.hpp>
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/timewheel/TimeWheelSystem.hpp"

namespace share::ecs::timewheel
{

std::unique_ptr<TimeWheelSystem>& TimeWheelSystem::GetInstance()
{
    static std::unique_ptr<TimeWheelSystem> ptr = nullptr;
    if (ptr == nullptr) {
        ptr = std::unique_ptr<TimeWheelSystem>(new TimeWheelSystem());
    }

    return ptr;
}

std::pair<std::optional<bbt::timer::Errcode>, TaskId>
    TimeWheelSystem::AddTask(engine::ecs::GameObjectSPtr gameobject, const std::function<bool()>& timeout_handle, int nframes)
{
    auto timewheel_comp = GetComponent(gameobject);   
    return timewheel_comp->AddTask(timeout_handle, nframes);
}

void TimeWheelSystem::CancelTask(engine::ecs::GameObjectSPtr gameobject, TaskId id)
{
    auto timewheel_comp = GetComponent(gameobject);   
    timewheel_comp->CancelTask(id);
}

std::shared_ptr<TimeWheelComp> TimeWheelSystem::GetComponent(engine::ecs::GameObjectSPtr gameobject)
{
    auto comp = gameobject->GetComponent(EM_COMPONENT_TYPE_TIME_WHEEL);
    if (comp == nullptr)
        return nullptr;
    
    auto timewheel_comp = std::static_pointer_cast<TimeWheelComp>(comp);
    return timewheel_comp;
}

void TimeWheelSystem::InitGameobject(engine::ecs::GameObjectSPtr gameobject, int fps)
{
    auto timewheel_comp = G_ComponentMgr()->Create<TimeWheelComp>(fps);
    Assert(gameobject->AddComponent(timewheel_comp));
}


}