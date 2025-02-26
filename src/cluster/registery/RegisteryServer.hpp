#pragma once
#include <util/network/TcpServer.hpp>


namespace cluster
{

class Registery;

class RegisteryServer:
    public util::network::TcpServer
{
public:
    RegisteryServer(
        std::shared_ptr<bbt::network::libevent::Network> network,
        std::shared_ptr<Registery> base,
        const std::string& ip,
        short port,
        int connect_timeout);
    virtual ~RegisteryServer() = default;
protected:
    std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override;
private:
    std::weak_ptr<Registery> m_registery_weak;
    const int m_connect_timeout{-1};
};

}