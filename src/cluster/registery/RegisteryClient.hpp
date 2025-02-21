#pragma once
#include <util/network/TcpClient.hpp>
#include <cluster/connection/NRConnection.hpp>

namespace cluster
{
class RegisteryClient:
    public util::network::TcpClient
{
public:
    RegisteryClient(const bbt::net::IPAddress& addr):
        util::network::TcpClient(addr.GetIP().c_str(), addr.GetPort(), 5000)
    {
    }

    virtual ~RegisteryClient() = default;
};

} // namespace cluster