#pragma once
#include <engine/ecs/system/System.hpp>
#include <plugin/ecs/network/ServerComp.hpp>


namespace plugin::ecs::network
{
typedef std::function<std::shared_ptr<Connection>(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn)> OnAsyncConnectCallback;

class NetworkSystem:
    public engine::ecs::System<NetworkSystem>
{
    typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
public:
    bool InitNetwork(GameObjectSPtr gameobject, const std::string& ip, short port, int connect_timeout);
    bool StartNetwork(GameObjectSPtr gameobject);
    bool StopNetwork(GameObjectSPtr gameobject);

    bool AsyncConnect(GameObjectSPtr gameobject, const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect);
private:
    std::shared_ptr<Server> GetComponent(GameObjectSPtr gameobject);

};
} // namespace share::ecs::network