#include <cluster/node/ClusterNode.hpp>

namespace cluster
{

ClusterNode::ClusterNode(const std::string& service_name):
    ClusterNodeBase(addr, service_name)
{
}

void ClusterNode::Init(const bbt::net::IPAddress& addr, int timeout)
{
    m_rpc_server = std::make_shared<RpcServer>(shared_from_this(), addr, timeout);
}


} // namespace cluster