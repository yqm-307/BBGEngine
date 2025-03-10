#pragma once
#include <functional>
#include <bbt/core/buffer/Buffer.hpp>
#include <util/errcode/ErrCodeDef.hpp>
#include <util/other/Uuid.hpp>
#include <cluster/ClusterDefine.hpp>
#include <cluster/protocol/Protocol.hpp>

namespace cluster
{   

class RpcClient;
class RpcServer;

typedef std::function<util::errcode::ErrOpt(bbt::core::Buffer&)> RpcReplyCallback;
typedef std::function<util::errcode::ErrOpt(bbt::core::Buffer&/*req*/, bbt::core::Buffer&/*resp*/)> RpcCallback;

} // namespace cluster::rpc