#pragma once
#include "share/ecs/Component.hpp"

namespace game::share::ecs
{
class GameObject: public bbt::templateutil::BaseType<GameObject>
{
public:
    GameObject(){}
    virtual ~GameObject() = 0;
    virtual void OnCreate(){};
    virtual void OnDestory(){};
    virtual void OnUpdate() = 0;
};

}