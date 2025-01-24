#pragma once
#include <string>

namespace share::ecs::cluster
{

enum NodeState
{
    NODESTATE_DEFAULT = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

}