#pragma once
#include "engine/ecs/system/System.hpp"
#include "plugin/ecs/timewheel/TimeWheelComp.hpp"

namespace share::ecs::timewheel
{

class TimeWheelSystem:
    public engine::ecs::System<TimeWheelSystem>
{
public:
    TimeWheelSystem() {}
    ~TimeWheelSystem() {}

    static std::unique_ptr<TimeWheelSystem>& GetInstance();

    void InitGameobject(engine::ecs::GameObjectSPtr gameobject, int fps);
    // n 帧后触发
    std::pair<std::optional<bbt::timer::Errcode>, TaskId>
         AddTask(engine::ecs::GameObjectSPtr gameobject, const std::function<bool()>& timeout_handle, int nframes);

    void CancelTask(engine::ecs::GameObjectSPtr gameobject, TaskId id);
private:
    std::shared_ptr<TimeWheelComp> GetComponent(engine::ecs::GameObjectSPtr gameobject);
};

}