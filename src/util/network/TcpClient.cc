#include <util/log/Log.hpp>
#include <util/network/TcpClient.hpp>

namespace util::network
{

TcpClient::TcpClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const char* ip, short port, int timeout):
    m_network(network),
    m_server_addr(ip, port)
{
}

util::errcode::ErrOpt TcpClient::AsyncConnect(const bbt::network::interface::OnConnectCallback& on_connect)
{
    if (m_network == nullptr)
        return util::errcode::ErrCode{"[TcpClient] network not found!", util::errcode::CommonErr};

    auto err = m_network->AsyncConnect(m_server_addr.GetIP().c_str(), m_server_addr.GetPort(), m_connect_timeout, on_connect);
    if (err.IsErr()) {
        return util::errcode::ErrCode{"[TcpClient] async connect failed!!", util::errcode::CommonErr};
    }

    return std::nullopt;
}

void TcpClient::SetConn(std::shared_ptr<util::network::Connection> conn)
{
    m_conn = conn;
}

void TcpClient::DelConn()
{
    m_conn = nullptr;
}

std::shared_ptr<util::network::Connection> TcpClient::GetConn() const
{
    return m_conn;
}

}