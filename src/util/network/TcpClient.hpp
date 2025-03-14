#pragma once
#include <util/network/Connection.hpp>

namespace util::network
{

typedef std::function<void(util::errcode::ErrOpt, std::shared_ptr<TcpClient>)> TcpClientOnConnectCallback;

class TcpClient:
    public std::enable_shared_from_this<TcpClient>
{
public:
    TcpClient(std::shared_ptr<bbt::network::base::NetworkBase> network);

    util::errcode::ErrOpt Init(
        const util::network::IPAddress& serv_addr,
        ConnectionCreator creator,
        int timeout,
        const TcpClientOnConnectCallback& on_connect);

    util::errcode::ErrOpt AsyncConnect();
    std::shared_ptr<util::network::Connection> GetConn();
    bbt::network::ConnId GetConnId() const;

private:
    void _OnConnect(util::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn);
    void _DelConn();
    void _AddConn(std::shared_ptr<util::network::Connection>);
private:
    std::shared_ptr<bbt::network::base::NetworkBase> m_network{nullptr};
    util::network::IPAddress                m_server_addr;
    std::shared_ptr<util::network::Connection> m_conn{nullptr};
    int                                     m_connect_timeout{1000};
    TcpClientOnConnectCallback              m_on_connect{nullptr};
    std::atomic_bool                        m_is_connecting{false};
    ConnectionCreator                       m_conn_creator{nullptr};
    std::mutex                              m_conn_lock;
};

}