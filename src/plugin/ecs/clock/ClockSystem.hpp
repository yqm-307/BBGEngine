#pragma once
#include <engine/ecs/system/System.hpp>
#include "plugin/ecs/clock/ClockComp.hpp"


namespace plugin::ecs::clock
{

class ClockSystem:
    public engine::ecs::System
{
public:
    int64_t                 GetFrame(engine::ecs::GameObjectSPtr gameobject);
    bbt::clock::Timestamp<> GetFrameTime(GameObjectSPtr gameobject);
private:
    std::shared_ptr<ClockComp> GetComponent(GameObjectSPtr gameobject);
};

}