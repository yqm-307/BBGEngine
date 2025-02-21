#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/timewheel/TimeWheel.hpp"
#include "plugin/ecs/timewheel/TimeWheelComp.hpp"

namespace plugin::ecs::timewheel
{

TimeWheel::~TimeWheel()
{}

TimeWheel::TimeWheel(int fps):
    engine::ecs::Entity(EM_ENTITY_TYPE_TIME_WHEEL)
{

}

}