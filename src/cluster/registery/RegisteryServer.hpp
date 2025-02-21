#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/connection/NRConnection.hpp>

namespace cluster
{

class RegisteryServer:
    public util::network::TcpServer
{
public:
    RegisteryServer(RegisteryBase* base, const std::string& ip, short port, int connect_timeout);
    virtual ~RegisteryServer() = default;
protected:
    std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override
    {
        return std::make_shared<NRConnection>(m_registery, conn, 5000);
    }
private:
    RegisteryBase* m_registery{nullptr};
};

}