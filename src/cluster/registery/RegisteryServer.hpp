#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/connection/NRConnection.hpp>

namespace cluster
{

class RegisteryServer:
    public util::network::TcpServer
{
protected:
    std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override
    {
        return std::make_shared<NRConnection>(conn, 5000);
    }
private:
};

}