#pragma once
#include <util/network/TcpServer.hpp>

namespace cluster
{

class RegisteryServer:
    public util::network::TcpServer
{
public:
    RegisteryServer(std::weak_ptr<RegisteryBase> base, const std::string& ip, short port, int connect_timeout);
    virtual ~RegisteryServer() = default;
protected:
    std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override;
private:
    std::weak_ptr<RegisteryBase> m_registery_weak;
};

}