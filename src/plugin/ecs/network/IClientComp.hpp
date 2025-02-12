#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>
#include <engine/ecs/component/Component.hpp>
#include <plugin/ecs/Define.hpp>

namespace plugin::ecs::network
{

class IClientComp:
    public engine::ecs::Component
{
    ComponentClassMetaInfo(plugin::ecs::EM_COMPONENT_TYPE_CLIENT);
public:

};

}