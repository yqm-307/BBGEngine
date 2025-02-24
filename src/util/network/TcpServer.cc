#include <util/log/Log.hpp>
#include <util/network/TcpServer.hpp>

namespace util::network
{

TcpServer::TcpServer(std::shared_ptr<bbt::network::base::NetworkBase> network, const std::string& ip, short port, int connect_timeout):
    m_network(network),
    m_listen_addr(ip, port),
    m_connect_timeout(connect_timeout)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::SetListenAddr(const char* ip, short port)
{
    Assert(m_network != nullptr);
    auto auto_init_io_thread_err = m_network->AutoInitThread(2);
    if (auto_init_io_thread_err.IsErr()) {
        GAME_BASE_LOG_ERROR("NetworkComponent::SetListenAddr init failed!");
        return;
    }

    auto err = m_network->StartListen(ip, port, [this](auto err, auto conn){ OnAccept(err, conn); });
    if (err.IsErr())
        GAME_BASE_LOG_ERROR("start listen failed! %s", err.CWhat());
}

void TcpServer::Init()
{
    SetListenAddr(m_listen_addr.GetIP().c_str(), m_listen_addr.GetPort());
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

void TcpServer::OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn)
{
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("accept failed! %s", err.CWhat());
        return;
    }
    
    auto conn = CreateConnection(new_conn);
    if (!AddConnect(conn)) {
        GAME_EXT1_LOG_ERROR("add connect failed!");
        return;
    }

    conn->SetOnClose([this](auto conn_id){
        DelConnect(conn_id);
    });

    GAME_EXT1_LOG_INFO("new connection: {%s}", new_conn->GetPeerAddress().GetIPPort().c_str());
}

bool TcpServer::Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto err = m_network->AsyncConnect(ip, port, timeout, on_connect);
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("AsyncConnect() failed! %s", err.CWhat());
        return false;
    }

    return true;
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

void TcpServer::OnTimeout(Connection* conn)
{
    if (!DelConnect(conn->GetConnId())) {
        GAME_EXT1_LOG_ERROR("[ConnMgr::OnTimeout] connect timeout!");
    }
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
        GAME_EXT1_LOG_ERROR("conn is nullptr!");
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
        GAME_EXT1_LOG_ERROR("conn is nullptr!");
        return;
    }

    conn_ptr->Close();
}

const bbt::net::IPAddress& TcpServer::GetListenAddr() const
{
    return m_listen_addr;
}

} // namespace util::network