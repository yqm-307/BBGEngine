#pragma once
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include <bbt/core/buffer/Buffer.hpp>
#include <bbt/core/clock/Clock.hpp>
#include <bbt/network/Define.hpp>

#include <util/other/Uuid.hpp>
#include <util/errcode/ErrCodeDef.hpp>

#include <cluster/ClusterDefine.hpp>
#include <cluster/protocol/Protocol.hpp>


// 心跳的间隔时间
#define BBGENGINE_HEARTBEAT_TIMEOUT_MS 3000

// 所有的连接默认超时时间都是这个
#define BBGENGINE_CLUSTER_CONN_FREE_TIMEOUT 10000

// async connect 超时时间
#define BBGENGINE_CONNECT_TIMEOUT 1000

// 重连时间间隔
#define BBGENGINE_RECONNECT_TIME 10000

namespace cluster
{

class NodeMgr;
class RpcClient;
class RpcServer;

using namespace bbt::core::net;

enum NodeState
{
    NODESTATE_UNREGISTER = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

typedef std::function<util::errcode::ErrOpt(bbt::core::Buffer&)> RpcReplyCallback;
typedef std::function<util::errcode::ErrOpt(bbt::core::Buffer&/*req*/, bbt::core::Buffer&/*resp*/)> RpcCallback;
typedef int64_t RpcMethodHash;

}