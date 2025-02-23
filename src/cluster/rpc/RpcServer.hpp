#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/rpc/Define.hpp>
#include <cluster/connection/NNConnection.hpp>

namespace cluster
{

class ClusterNodeBase;

// FIXME 这里设计有问题，继承自TcpServer的RpcServer具备发送和接收的功能，应该是一个具体类
class RpcServer:
    public util::network::TcpServer
{
    friend class RpcClient;
public:
    RpcServer(std::shared_ptr<ClusterNodeBase> node, const bbt::net::IPAddress& listen_addr, int timeout);
    virtual ~RpcServer();

    int                     Register(const std::string& method, RpcCallback callback);
    util::errcode::ErrOpt   OnRpc(bbt::core::Buffer& buffer);
    virtual void            Send(const char* data, size_t size) = 0;
    virtual std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override;

private:
    std::unordered_map<std::string, RpcCallback> m_registed_methods;    // 注册的服务方法
    std::weak_ptr<ClusterNodeBase> m_node_weak;
};

} // namespace cluster::rpc