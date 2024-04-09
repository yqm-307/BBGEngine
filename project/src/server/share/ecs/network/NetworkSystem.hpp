#pragma once
#include "share/ecs/network/NetworkComponent.hpp"
#include "share/ecs/network/ConnMgr.hpp"


namespace share::ecs::network
{

class NetworkSystem
{
    typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
public:
    ~NetworkSystem() {}
    static std::unique_ptr<NetworkSystem>& GetInstance();

    bool InitNetwork(GameObjectSPtr gameobject, const ServerCfg& cfg);
    bool StartNetwork(GameObjectSPtr gameobject);
    bool StopNetwork(GameObjectSPtr gameobject);

private:
    NetworkSystem() {}
    std::shared_ptr<NetworkComponent> GetComponent(GameObjectSPtr gameobject);

};
} // namespace share::ecs::network