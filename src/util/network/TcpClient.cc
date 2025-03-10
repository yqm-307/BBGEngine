#include <util/network/TcpClient.hpp>
#include <util/errcode/ErrCodeDef.hpp>

namespace util::network
{

TcpClient::TcpClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const char* ip, short port, int timeout):
    m_network(network),
    m_server_addr(ip, port)
{
}

util::errcode::ErrOpt TcpClient::AsyncConnect(const std::function<void(util::errcode::ErrOpt, std::shared_ptr<TcpClient>)>& on_connect)
{
    if (m_network == nullptr)
        return util::errcode::Errcode{"[TcpClient] network not found!", util::errcode::CommonErr};

    if (m_on_connect != nullptr)
        return util::errcode::Errcode{"[TcpClient] already connect!", util::errcode::CommonErr};
    m_on_connect = on_connect;

    auto err = m_network->AsyncConnect(m_server_addr.GetIP().c_str(), m_server_addr.GetPort(), m_connect_timeout, 
    [weak_this{weak_from_this()}](auto err, auto conn){
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->_OnConnect(err, conn);
    });
    if (err.has_value()) {
        return util::errcode::Errcode{"[TcpClient] async connect failed!! " + err->What(), util::errcode::CommonErr};
    }

    return std::nullopt;
}

void TcpClient::_OnConnect(util::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn)
{
    if (err.has_value()) {
        // OnError(err.value());
        return;
    }

    m_conn = m_conn_creator(std::static_pointer_cast<bbt::network::libevent::Connection>(conn));
    if (m_conn == nullptr) {
        return;
    }

    m_conn->SetOnCloseNotifyToTcpServer([weak_this{weak_from_this()}](auto connid){
        if (auto shared_this = weak_this.lock(); shared_this != nullptr)
            shared_this->_DelConn();
    });

    m_conn->Init();
    _AddConn(m_conn);

    m_on_connect(err, shared_from_this());
    m_on_connect = nullptr;
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

util::errcode::ErrOpt TcpClient::Init(ConnectionCreator creator)
{
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