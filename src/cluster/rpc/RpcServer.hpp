#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/rpc/Define.hpp>

namespace cluster
{

class ClusterNode;

class RpcServer
{
    friend class RpcClient;
public:
    RpcServer(std::shared_ptr<ClusterNode> node, const util::network::IPAddress& listen_addr, int timeout);
    virtual ~RpcServer() noexcept;

    int                         Register(const std::string& method, RpcCallback callback);
    util::errcode::ErrOpt       OnRemoteCall(bbt::core::Buffer& req);
    std::vector<std::string>    GetRegistedMethods() const;
    bool                        HasMethod(const std::string& method) const;
private:
    std::unordered_map<std::string, RpcCallback> m_registed_methods;    // 注册的服务方法
    std::weak_ptr<ClusterNode> m_node_weak;
};

} // namespace cluster::rpc