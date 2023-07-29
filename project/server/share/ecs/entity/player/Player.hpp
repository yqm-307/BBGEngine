#pragma once
#include "share/ecs/GameObject.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::entity
{

class Player:
    public GameObject
{
public:
    Player();
    ~Player();
    virtual void OnUpdate();
private:
};

}