#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>
#include "share/ecs/network/NetworkDef.hpp"

namespace share::ecs::network
{

class NetworkComponent:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    ~NetworkComponent();

    void SetListenAddr(const char* ip, short port);
    void Start();
    void Stop();

private:
    NetworkComponent();
    virtual void OnUpdate() override {}
    void OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn);

private:
    bbt::network::libevent::Network*    m_network{nullptr};
};

} // namespace share::ecs::network