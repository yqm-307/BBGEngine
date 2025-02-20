#pragma once
#include <util/network/Connection.hpp>

namespace util::network
{

class TcpClient
{
public:
    TcpClient(const char* ip, short port, int timeout);

    bool AsyncConnect(const bbt::network::interface::OnConnectCallback& on_connect);
private:
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};
    bbt::net::IPAddress m_server_addr;
    int m_connect_timeout{1000};
};

}