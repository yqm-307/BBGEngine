#pragma once
#include "share/ecs/GameObject.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::gameobject
{

class Player:
    public GameObject
{
public:
    Player();
    ~Player();
private:
    // 静态定义的Component
    component::AoiComponent::SPtr m_com_aoi; 
    // 动态挂在的Component    

};

}