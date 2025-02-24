#pragma once
#include <util/network/TcpClient.hpp>

namespace cluster
{
class RegisteryClient:
    public util::network::TcpClient
{
public:
    RegisteryClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const bbt::net::IPAddress& addr):
        util::network::TcpClient(network, addr.GetIP().c_str(), addr.GetPort(), 5000)
    {
    }

    virtual ~RegisteryClient() = default;
};

} // namespace cluster