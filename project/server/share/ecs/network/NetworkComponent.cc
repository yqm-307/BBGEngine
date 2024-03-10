#include "share/ecs/Define.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

NetworkComponent::NetworkComponent():
    engine::ecs::Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_NETWORK)
{
    m_network = new bbt::network::libevent::Network{2};
}

NetworkComponent::~NetworkComponent()
{
    delete m_network;
    m_network = nullptr;
}

void NetworkComponent::SetListenAddr(const char* ip, short port)
{
    Assert(m_network != nullptr);
    auto err = m_network->StartListen(ip, port, [this](auto err, auto conn){ OnAccept(err, conn); });
    if (!err)
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
    if (!err) {
        GAME_EXT1_LOG_ERROR("accept failed! %s", err.CWhat());
        return;
    }
    GAME_EXT1_LOG_INFO("new connection: {%s}", conn->GetPeerAddress().GetIPPort().c_str());
}

}