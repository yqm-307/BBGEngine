#pragma once
#include <string>

namespace plugin::ecs::cluster
{

enum NodeState
{
    NODESTATE_DEFAULT = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

}