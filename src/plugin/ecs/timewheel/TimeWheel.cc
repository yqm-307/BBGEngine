#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/timewheel/TimeWheel.hpp"
#include "plugin/ecs/timewheel/TimeWheelComp.hpp"

namespace share::ecs::timewheel
{

TimeWheel::~TimeWheel()
{}

TimeWheel::TimeWheel(int fps):
    engine::ecs::GameObject(EM_ENTITY_TYPE_TIME_WHEEL)
{

}

}