#pragma once
#include <util/network/Connection.hpp>

namespace util::network
{

class TcpClient:
    public std::enable_shared_from_this<TcpClient>
{
public:
    TcpClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const char* ip, short port, int timeout);

    util::errcode::ErrOpt AsyncConnect(const bbt::network::interface::OnConnectCallback& on_connect);

    void SetConn(std::shared_ptr<util::network::Connection> conn);
    void DelConn();
    std::shared_ptr<util::network::Connection> GetConn() const;
private:
    std::shared_ptr<bbt::network::base::NetworkBase> m_network{nullptr};
    bbt::net::IPAddress m_server_addr;
    std::shared_ptr<util::network::Connection> m_conn{nullptr};
    int m_connect_timeout{1000};
};

}