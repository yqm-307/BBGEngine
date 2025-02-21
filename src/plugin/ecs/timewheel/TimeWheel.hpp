#pragma once
#include <engine/ecs/entity/Entity.hpp>

namespace plugin::ecs::timewheel
{

class TimeWheel:
    public engine::ecs::Entity
{
    GameObjectClassMetaInfo;
public:
    ~TimeWheel();
protected:
    /* 游戏每秒fps */
    TimeWheel(int one_second_game_fps);
private:
};

}