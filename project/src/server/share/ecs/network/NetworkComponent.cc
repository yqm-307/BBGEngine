#include "share/ecs/Define.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

NetworkComponent::NetworkComponent():
    engine::ecs::Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_NETWORK)
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
}