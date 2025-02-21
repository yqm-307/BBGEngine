#pragma once
#include <string>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cluster/ClusterDefine.hpp>
#include <util/other/Uuid.hpp>
#include <bbt/base/buffer/Buffer.hpp>
#include <bbt/base/clock/Clock.hpp>
#include <util/errcode/ErrCode.hpp>


namespace cluster
{

enum NodeState
{
    NODESTATE_DEFAULT = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

struct NodeRegInfo
{

};

#pragma pack(push, 1)

struct N2R_KeepAlive
{
};

struct N2R_KeepAliveAck
{
};

struct N2R_RegService
{
};

#pragma pack(pop)



}