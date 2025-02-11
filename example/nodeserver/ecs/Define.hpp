#pragma once
#include <plugin/ecs/Define.hpp>

namespace BBTENGINE_NODE_NAMESPACE::ecs
{

enum COMPONENT_TYPE : engine::ecs::GameObjectTemplateId
{
    COMPONENT_TYPE_BEGIN = plugin::ecs::EM_COMPONENT_TYPE_MAX_COUNT,
    COMPONENT_TYPE_ECHO_SERVICE,
};

}