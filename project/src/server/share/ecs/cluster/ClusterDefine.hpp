#pragma once
#include <string>

namespace share::ecs::cluster
{

enum NodeState
{
    NODESTATE_ONLINE = 1,
    NODESTATE_OFFLINE = 2,
};

struct NodeInfo
{
    std::string     ip;
    short           port;
    NodeState       state;
    std::string     service_name;
    time_t          last_active_time{-1};
};


}