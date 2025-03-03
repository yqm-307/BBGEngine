#pragma once
#include <string>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cluster/ClusterDefine.hpp>
#include <util/other/Uuid.hpp>
#include <bbt/base/buffer/Buffer.hpp>
#include <bbt/base/clock/Clock.hpp>


namespace cluster
{

class NodeMgr;

#define HEARTBEAT_TIMEOUT_MS 3000

enum NodeState
{
    NODESTATE_UNREGISTER = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

}