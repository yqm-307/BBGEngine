#include "share/ecs/Define.hpp"
#include "share/ecs/timewheel/TimeWheel.hpp"
#include "share/ecs/timewheel/TimeWheelComp.hpp"

namespace share::ecs::timewheel
{

TimeWheel::~TimeWheel()
{}

TimeWheel::TimeWheel(int fps):
    engine::ecs::GameObject(EM_ENTITY_TYPE_TIME_WHEEL)
{

}

}