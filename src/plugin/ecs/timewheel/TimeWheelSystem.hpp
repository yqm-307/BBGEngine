#pragma once
#include "engine/ecs/system/System.hpp"
#include "plugin/ecs/timewheel/TimeWheelComp.hpp"

namespace plugin::ecs::timewheel
{

class TimeWheelSystem:
    public engine::ecs::System
{
public:
    virtual void OnUpdate() override {}

    static std::unique_ptr<TimeWheelSystem>& GetInstance();

    void InitGameobject(engine::ecs::EntitySPtr gameobject, int fps);
    // n 帧后触发
    std::pair<std::optional<bbt::timer::Errcode>, TaskId>
         AddTask(engine::ecs::EntitySPtr gameobject, const std::function<bool()>& timeout_handle, int nframes);

    void CancelTask(engine::ecs::EntitySPtr gameobject, TaskId id);
private:
    std::shared_ptr<TimeWheelComp> GetComponent(engine::ecs::EntitySPtr gameobject);
};

}