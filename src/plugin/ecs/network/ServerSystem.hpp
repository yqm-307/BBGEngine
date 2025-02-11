#pragma once
#include <engine/ecs/system/System.hpp>
#include <plugin/ecs/network/ServerComp.hpp>


namespace plugin::ecs::network
{
typedef std::function<std::shared_ptr<Connection>(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn)> OnAsyncConnectCallback;

template<class TServerComp>
class ServerSystem:
    public engine::ecs::System<ServerSystem<TServerComp>>
{
    typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
    static_assert(std::is_base_of_v<Server, TServerComp>, "TServerComp must be derived from Server");

public:
    bool StartNetwork(GameObjectSPtr gameobject);
    bool StopNetwork(GameObjectSPtr gameobject);

    bool AsyncConnect(GameObjectSPtr gameobject, const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect);
    size_t  Send(GameObjectSPtr gameobject, bbt::network::ConnId id, const char* bytes, size_t len);
    void    Close(GameObjectSPtr gameobject, bbt::network::ConnId id);

};
} // namespace share::ecs::network

#include <plugin/ecs/network/__TServerSystem.hpp>