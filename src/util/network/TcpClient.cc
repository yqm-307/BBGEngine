#include <util/log/Log.hpp>
#include <util/network/TcpClient.hpp>

namespace util::network
{

TcpClient::TcpClient(const char* ip, short port, int timeout):
    m_network(std::make_shared<bbt::network::libevent::Network>()),
    m_server_addr(ip, port)
{
}

bool TcpClient::AsyncConnect(const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto err = m_network->AsyncConnect(m_server_addr.GetIP().c_str(), m_server_addr.GetPort(), m_connect_timeout, on_connect);
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("AsyncConnect() failed! %s", err.CWhat());
        return false;
    }

    return true;
}


}