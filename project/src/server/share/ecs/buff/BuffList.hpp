#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/buff/BuffBase.hpp"


namespace share::ecs::buff
{

class BuffList:
    public engine::ecs::Component
{
private:
    virtual void OnUpdate() override {};
};

}