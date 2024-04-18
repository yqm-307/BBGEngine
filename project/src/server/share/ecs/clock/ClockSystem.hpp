#pragma once
#include <engine/ecs/system/System.hpp>
#include "share/ecs/clock/ClockComp.hpp"


namespace share::ecs::clock
{

class ClockSystem:
    public engine::ecs::System<ClockSystem>
{
public:
    int64_t                 GetFrame(GameObjectSPtr gameobject);
    bbt::clock::Timestamp<> GetFrameTime(GameObjectSPtr gameobject);
private:
    std::shared_ptr<ClockComp> GetComponent(GameObjectSPtr gameobject);
};

}