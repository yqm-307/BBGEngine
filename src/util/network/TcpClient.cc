#include <util/network/TcpClient.hpp>
#include <util/errcode/ErrCodeDef.hpp>

namespace util::network
{

TcpClient::TcpClient(std::shared_ptr<bbt::network::base::NetworkBase> network):
    m_network(network)
{
}

util::errcode::ErrOpt TcpClient::AsyncConnect()
{
    if (m_network == nullptr)
        return util::errcode::Errcode{"[TcpClient] network not found!", util::errcode::CommonErr};
    
    bool expected = false;
    if (!m_is_connecting.compare_exchange_strong(expected, true))
        return util::errcode::Errcode{"[TcpClient] is connecting!", util::errcode::CommonErr};

    auto err = m_network->AsyncConnect(m_server_addr.GetIP().c_str(), m_server_addr.GetPort(), m_connect_timeout, 
    [weak_this{weak_from_this()}](auto err, auto conn){
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->_OnConnect(err, conn);
    });

    return err;
}

void TcpClient::_OnConnect(util::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn)
{
    if (!err && conn != nullptr) {
        if (m_conn = m_conn_creator(std::static_pointer_cast<bbt::network::libevent::Connection>(conn)); m_conn != nullptr) {
            m_conn->SetOnCloseNotifyToTcpServer([weak_this{weak_from_this()}](auto connid){
                if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                    shared_this->_DelConn();
            });
            m_conn->Init();
            _AddConn(m_conn);
        }
    }

    bool expected = true;
    m_is_connecting.compare_exchange_strong(expected, false);

    m_on_connect(err, shared_from_this());
}

void TcpClient::_DelConn()
{
    std::lock_guard<std::mutex> lock{m_conn_lock};
    m_conn = nullptr;
}

void TcpClient::_AddConn(std::shared_ptr<util::network::Connection> conn)
{
    std::lock_guard<std::mutex> lock{m_conn_lock};
    m_conn = conn;
}

util::errcode::ErrOpt TcpClient::Init(
    const util::network::IPAddress& serv_addr,
    ConnectionCreator creator,
    int timeout,
    const TcpClientOnConnectCallback& on_connect)
{
    bool expected = false;
    if (m_is_connecting)
        return util::errcode::Errcode{"[TcpClient] is connecting!", util::errcode::CommonErr};

    m_server_addr = serv_addr;
    m_connect_timeout = timeout;
    m_on_connect = on_connect;
    m_conn_creator = creator;
    return std::nullopt;
}

std::shared_ptr<util::network::Connection> TcpClient::GetConn()
{
    std::lock_guard<std::mutex> lock{m_conn_lock};
    return m_conn;
}

bbt::network::ConnId TcpClient::GetConnId() const
{
    if (m_conn == nullptr)
        return 0;
    return m_conn->GetConnId();
}

}