#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>
#include "plugin/ecs/network/NetworkDef.hpp"
#include "engine/ecs/component/Component.hpp"
#include "plugin/ecs/network/Connection.hpp"
namespace share::ecs::network
{

class NetworkComponent:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    ~NetworkComponent();

    virtual void Init() override;
    void SetListenAddr(const char* ip, short port);
    void Start();
    void Stop();
    //TODO
    bool Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect);
    void SetOnAccept(const bbt::network::libevent::OnAcceptCallback& onaccept_cb);

    bool DelConnect(bbt::network::ConnId conn);
    bool AddConnect(std::shared_ptr<Connection> conn);
    void OnTimeout(Connection* conn);
    std::shared_ptr<Connection> GetConnectById(bbt::network::ConnId conn_id); 

private:
    NetworkComponent(const std::string& ip, short port, int connect_timeout);
    virtual void OnUpdate() override {}
    void OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn);

private:
    bbt::network::libevent::Network*        m_network{nullptr};
    bbt::network::libevent::OnAcceptCallback m_onaccept_cb{nullptr};
    std::unordered_map<engine::ecs::ComponentId, std::shared_ptr<Connection>>
                                            m_conn_map;
    bbt::net::IPAddress                     m_listen_addr;
    int                                     m_connect_timeout{1000};
};

} // namespace share::ecs::network