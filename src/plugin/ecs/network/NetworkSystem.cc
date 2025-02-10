#include "engine/ecs/gameobject/GameObject.hpp"
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/network/NetworkSystem.hpp"

namespace share::ecs::network
{

bool NetworkSystem::InitNetwork(GameObjectSPtr gameobject, const std::string& ip, short port, int connect_timeout)
{
    Assert(gameobject->AddComponent<share::ecs::network::NetworkComponent>(ip, port, connect_timeout));
    return true;
}


bool NetworkSystem::StartNetwork(GameObjectSPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return false;

    comp->Start();
    return true;
}

bool NetworkSystem::StopNetwork(GameObjectSPtr gameobject)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return false;

    comp->Stop();
    return true;
}

std::shared_ptr<NetworkComponent> NetworkSystem::GetComponent(GameObjectSPtr obj)
{
    if (obj == nullptr)
        return nullptr;
    
    auto component = obj->GetComponent(share::ecs::emComponentType::EM_COMPONENT_TYPE_NETWORK);
    if (component == nullptr)
        return nullptr;
    
    return std::static_pointer_cast<ecs::network::NetworkComponent>(component);
}

bool NetworkSystem::AsyncConnect(GameObjectSPtr gameobject, const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto component = gameobject->GetComponent(EM_COMPONENT_TYPE_NETWORK);
    if (component == nullptr)
        return false;

    auto network_comp = std::static_pointer_cast<NetworkComponent>(component);
    if (network_comp == nullptr)
        return false;
    
    return network_comp->Connect(ip, port, timeout, on_connect);
}

}