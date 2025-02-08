#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>
#include "plugin/ecs/network/NetworkDef.hpp"

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
    //TODO
    bool Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect);
    void SetOnAccept(const bbt::network::libevent::OnAcceptCallback& onaccept_cb);

private:
    NetworkComponent();
    virtual void OnUpdate() override {}
    void OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn);

private:
    bbt::network::libevent::Network*    m_network{nullptr};
    bbt::network::libevent::OnAcceptCallback m_onaccept_cb{nullptr};
};

} // namespace share::ecs::network