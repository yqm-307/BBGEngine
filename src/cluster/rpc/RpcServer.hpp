#pragma once
#include <cluster/rpc/Define.hpp>

namespace cluster
{

class RpcServer
{
    friend class RpcClient;
public:
    RpcServer();
    ~RpcServer();

    int Register(const std::string& method, RpcCallback callback);

    util::errcode::ErrOpt OnRpc(bbt::core::Buffer& buffer);

    virtual void Send(const char* data, size_t size) = 0;
private:

    std::unordered_map<std::string, RpcCallback> m_registed_methods;    // 注册的服务方法
};

} // namespace cluster::rpc