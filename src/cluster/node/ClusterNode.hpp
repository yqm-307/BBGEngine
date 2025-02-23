#pragma once
#include <cluster/node/ClusterNodeBase.hpp>

namespace cluster
{

class ClusterNode:
    public ClusterNodeBase
{
public:
    ClusterNode(const std::string& service_name);

    void                                Init(const bbt::net::IPAddress& addr, int timeout);

    
private:
    std::shared_ptr<RpcServer>          m_rpc_server{nullptr};
    std::unordered_map<std::string, std::shared_ptr<RpcClient>> m_rpc_clients;
    int connect_timeout{0};
};

} // namespace cluster