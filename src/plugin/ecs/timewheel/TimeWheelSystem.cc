#include <engine/ecs/entity/Entity.hpp>
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/timewheel/TimeWheelSystem.hpp"

namespace plugin::ecs::timewheel
{

std::pair<std::optional<bbt::timer::Errcode>, TaskId>
    TimeWheelSystem::AddTask(engine::ecs::EntitySPtr gameobject, const std::function<bool()>& timeout_handle, int nframes)
{
    auto timewheel_comp = GetComponent(gameobject);   
    return timewheel_comp->AddTask(timeout_handle, nframes);
}

void TimeWheelSystem::CancelTask(engine::ecs::EntitySPtr gameobject, TaskId id)
{
    auto timewheel_comp = GetComponent(gameobject);   
    timewheel_comp->CancelTask(id);
}

std::shared_ptr<TimeWheelComp> TimeWheelSystem::GetComponent(engine::ecs::EntitySPtr gameobject)
{
    auto comp = gameobject->GetComponent(EM_COMPONENT_TYPE_TIME_WHEEL);
    if (comp == nullptr)
        return nullptr;
    
    auto timewheel_comp = std::static_pointer_cast<TimeWheelComp>(comp);
    return timewheel_comp;
}

void TimeWheelSystem::InitGameobject(engine::ecs::EntitySPtr gameobject, int fps)
{
    // auto timewheel_comp = G_ComponentMgr()->Create<TimeWheelComp>(fps);
    // Assert(gameobject->AddComponent(timewheel_comp));
}


}