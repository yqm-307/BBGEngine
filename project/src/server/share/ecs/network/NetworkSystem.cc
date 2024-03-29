#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/Define.hpp"
#include "share/ecs/network/NetworkSystem.hpp"

namespace share::ecs::network
{

std::unique_ptr<NetworkSystem>& NetworkSystem::GetInstance()
{
    static std::unique_ptr<NetworkSystem> _inst = nullptr;
    if (_inst == nullptr) {
        _inst = std::unique_ptr<NetworkSystem>(new NetworkSystem());
    }

    return _inst;
}

bool NetworkSystem::InitNetwork(GameObjectSPtr gameobject, const char* ip, short port)
{
    auto comp = GetComponent(gameobject);
    if (comp == nullptr)
        return false;

    comp->SetListenAddr(ip, port);
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

}