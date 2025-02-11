#include <plugin/ecs/Define.hpp>
#include "plugin/ecs/network/ServerComp.hpp"

namespace plugin::ecs::network
{

Server::Server(const std::string& ip, short port, int connect_timeout):
    IServerComp(plugin::ecs::emComponentType::EM_COMPONENT_TYPE_SERVER),
    m_listen_addr(ip, port),
    m_connect_timeout(connect_timeout)
{
    m_network = new bbt::network::libevent::Network;
}

Server::~Server()
{
    delete m_network;
    m_network = nullptr;
}

void Server::SetListenAddr(const char* ip, short port)
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

void Server::Init()
{
    SetListenAddr(m_listen_addr.GetIP().c_str(), m_listen_addr.GetPort());
}

void Server::Start()
{
    Assert(m_network != nullptr);
    m_network->Start();
}

void Server::Stop()
{
    Assert(m_network != nullptr);
    m_network->Stop();
}

void Server::OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn)
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

bool Server::Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto err = m_network->AsyncConnect(ip, port, timeout, on_connect);
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("AsyncConnect() failed! %s", err.CWhat());
        return false;
    }

    return true;
}

bool Server::DelConnect(bbt::network::ConnId conn)
{
    auto it = m_conn_map.find(conn);
    if (it == m_conn_map.end()) {
        return false;
    }

    m_conn_map.erase(it);
    return true;
}

bool Server::AddConnect(std::shared_ptr<Connection> conn)
{
    if (conn == nullptr)
        return false;

    auto [it, succ] = m_conn_map.insert(std::make_pair(conn->GetConnId(), conn));
    return succ;
}

void Server::OnTimeout(Connection* conn)
{
    if (!DelConnect(conn->GetConnId())) {
        GAME_EXT1_LOG_ERROR("[ConnMgr::OnTimeout] connect timeout!");
    }
}

std::shared_ptr<Connection> Server::GetConnectById(bbt::network::ConnId conn_id)
{
    auto it = m_conn_map.find(conn_id);
    if (it == m_conn_map.end())
        return nullptr;

    return it->second;
}

size_t Server::Send(bbt::network::ConnId id, const char* bytes, size_t len)
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

size_t Server::Recv(bbt::network::ConnId conn, const char* recv_buf, size_t len)
{
    return 0;
}

void Server::ShowDown(bbt::network::ConnId conn)
{
    auto conn_ptr = GetConnectById(conn);
    if (conn_ptr == nullptr) {
        GAME_EXT1_LOG_ERROR("conn is nullptr!");
        return;
    }

    conn_ptr->Close();
}

} // namespace share::ecs::network