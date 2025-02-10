#pragma once
#include <engine/ecs/component/Component.hpp>
#include <bbt/network/adapter/libevent/Network.hpp>

namespace plugin::ecs::network
{

class IClientComp:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:

};

}