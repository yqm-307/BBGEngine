#include <util/network/TcpServer.hpp>
#include <util/errcode/ErrCodeDef.hpp>

namespace util::network
{

TcpServer::TcpServer(std::shared_ptr<bbt::network::libevent::Network> network, const std::string& ip, short port, int connect_timeout):
    m_network(network),
    m_listen_addr(ip, port),
    m_connect_timeout(connect_timeout)
{
}

TcpServer::~TcpServer()
{
}

util::errcode::ErrOpt TcpServer::SetListenAddr(const char* ip, short port)
{
    Assert(m_network != nullptr);
    auto auto_init_io_thread_err = m_network->AutoInitThread(2);
    if (auto_init_io_thread_err.has_value())
        return auto_init_io_thread_err;

    auto err = m_network->StartListen(ip, port, [this](auto err, auto conn){ OnAccept(err, conn); });
    if (err.has_value())
        return err;
    
    return std::nullopt;
}

void TcpServer::Init(ConnectionCreator creator)
{
    SetListenAddr(m_listen_addr.GetIP().c_str(), m_listen_addr.GetPort());
    m_conn_creator = creator;
}

void TcpServer::Start()
{
    Assert(m_network != nullptr);
    m_network->Start();
}

void TcpServer::Stop()
{
    Assert(m_network != nullptr);
    m_network->Stop();
}

void TcpServer::OnAccept(util::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr new_conn)
{
    //TODO ERROR
    if (err.has_value()) {
        // OnError(err.value());
        return;
    }
    
    auto conn = m_conn_creator(std::static_pointer_cast<bbt::network::libevent::Connection>(new_conn));
    if (!AddConnect(conn)) {
        // OnError(util::errcode::Errcode{"add connect failed!", util::errcode::NET_ACCEPT_ERR});
        return;
    }

    conn->SetOnCloseNotifyToTcpServer([this](auto conn_id){
        DelConnect(conn_id);
    });
}

bool TcpServer::DelConnect(bbt::network::ConnId conn)
{
    auto it = m_conn_map.find(conn);
    if (it == m_conn_map.end()) {
        return false;
    }

    m_conn_map.erase(it);
    return true;
}

bool TcpServer::AddConnect(std::shared_ptr<Connection> conn)
{
    if (conn == nullptr)
        return false;

    auto [it, succ] = m_conn_map.insert(std::make_pair(conn->GetConnId(), conn));
    return succ;
}

std::shared_ptr<Connection> TcpServer::GetConnectById(bbt::network::ConnId conn_id)
{
    auto it = m_conn_map.find(conn_id);
    if (it == m_conn_map.end())
        return nullptr;

    return it->second;
}

size_t TcpServer::Send(bbt::network::ConnId id, const char* bytes, size_t len)
{
    auto conn = GetConnectById(id);
    if (conn == nullptr) {
        return 0;
    }

    /*XXX 异步发送，目前是当做全部发送成功*/
    conn->Send(bytes, len);
    return len; 
}

size_t TcpServer::Recv(bbt::network::ConnId conn, const char* recv_buf, size_t len)
{
    return 0;
}

void TcpServer::ShowDown(bbt::network::ConnId conn)
{
    auto conn_ptr = GetConnectById(conn);
    if (conn_ptr == nullptr) {
        return;
    }

    conn_ptr->Close();
}

const util::network::IPAddress& TcpServer::GetListenAddr() const
{
    return m_listen_addr;
}

} // namespace util::network