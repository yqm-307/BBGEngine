#pragma once
#include <functional>
#include <bbt/base/buffer/Buffer.hpp>
#include <util/errcode/ErrCodeDef.hpp>
#include <plugin/ecs/Define.hpp>

namespace cluster
{   

class RpcClient;
class RpcServer;

typedef std::function<bbt::errcode::ErrOpt(bbt::core::Buffer&)> RpcReplyCallback;
typedef std::function<bbt::errcode::ErrOpt(bbt::core::Buffer&/*req*/, bbt::core::Buffer&/*resp*/)> RpcCallback;

} // namespace cluster::rpc