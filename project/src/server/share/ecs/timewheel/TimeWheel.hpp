#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>

namespace share::ecs::timewheel
{

class TimeWheel:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    ~TimeWheel();
protected:
    /* 游戏每秒fps */
    TimeWheel(int one_second_game_fps);
private:
};

}