#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>
#include "share/ecs/network/ConnMgr.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

class Network:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    virtual ~Network() {}
protected:
    Network();
    virtual void OnUpdate() override {}
private:
};

}