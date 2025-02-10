#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

NetworkComponent::NetworkComponent(const std::string& ip, short port, int connect_timeout):
    engine::ecs::Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_NETWORK),
    m_listen_addr(ip, port),
    m_connect_timeout(connect_timeout)
{
    m_network = new bbt::network::libevent::Network;
}

NetworkComponent::~NetworkComponent()
{
    delete m_network;
    m_network = nullptr;
}

void NetworkComponent::SetListenAddr(const char* ip, short port)
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

void NetworkComponent::Init()
{
    SetListenAddr(m_listen_addr.GetIP().c_str(), m_listen_addr.GetPort());
    SetOnAccept([this](auto err, auto new_conn){
        auto conn = std::make_shared<network::Connection>(new_conn, m_connect_timeout);
        if (!AddConnect(conn)) {
            GAME_EXT1_LOG_ERROR("add connect failed!");
            return;
        }

        conn->SetOnClose([this](auto conn_id){
            DelConnect(conn_id);
        });
    });
}

void NetworkComponent::Start()
{
    Assert(m_network != nullptr);
    m_network->Start();
}

void NetworkComponent::Stop()
{
    Assert(m_network != nullptr);
    m_network->Stop();
}

void NetworkComponent::OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn)
{
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("accept failed! %s", err.CWhat());
        return;
    }
    if (m_onaccept_cb == nullptr) {
        GAME_EXT1_LOG_WARN("no OnAccept()! lost connect: {%s}", conn->GetPeerAddress().GetIPPort().c_str());
        return;
    }
    m_onaccept_cb(err, conn);
    GAME_EXT1_LOG_INFO("new connection: {%s}", conn->GetPeerAddress().GetIPPort().c_str());
}

void NetworkComponent::SetOnAccept(const bbt::network::libevent::OnAcceptCallback& onaccept_cb)
{
    m_onaccept_cb = onaccept_cb;
}

bool NetworkComponent::Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto err = m_network->AsyncConnect(ip, port, timeout, on_connect);
    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR("AsyncConnect() failed! %s", err.CWhat());
        return false;
    }

    return true;
}

bool NetworkComponent::DelConnect(bbt::network::ConnId conn)
{
    auto it = m_conn_map.find(conn);
    if (it == m_conn_map.end()) {
        return false;
    }

    m_conn_map.erase(it);
    return true;
}

bool NetworkComponent::AddConnect(std::shared_ptr<Connection> conn)
{
    if (conn == nullptr)
        return false;

    auto [it, succ] = m_conn_map.insert(std::make_pair(conn->GetConnId(), conn));
    return succ;
}

void NetworkComponent::OnTimeout(Connection* conn)
{
    if (!DelConnect(conn->GetConnId())) {
        GAME_EXT1_LOG_ERROR("[ConnMgr::OnTimeout] connect timeout!");
    }
}

std::shared_ptr<Connection> NetworkComponent::GetConnectById(bbt::network::ConnId conn_id)
{
    auto it = m_conn_map.find(conn_id);
    if (it == m_conn_map.end())
        return nullptr;

    return it->second;
}

}