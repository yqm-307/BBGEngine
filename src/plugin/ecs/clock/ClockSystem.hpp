#pragma once
#include <engine/ecs/system/System.hpp>
#include "plugin/ecs/clock/ClockComp.hpp"


namespace plugin::ecs::clock
{

class ClockSystem:
    public engine::ecs::System
{
public:
    int64_t                 GetFrame(engine::ecs::EntitySPtr gameobject);
    bbt::core::clock::Timestamp<> GetFrameTime(EntitySPtr gameobject);
private:
    std::shared_ptr<ClockComp> GetComponent(EntitySPtr gameobject);
};

}