#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/network/ServerSystem.hpp>
#include <plugin/ecs/Define.hpp>

namespace plugin::ecs::network
{

template<class TServerComp>
bool ServerSystem<TServerComp>::StartNetwork(GameObjectSPtr gameobject)
{
    auto comp = gameobject->GetComponent<TServerComp>();
    if (comp == nullptr)
        return false;    

    comp->Start();
    return true;
}

template<class TServerComp>
bool ServerSystem<TServerComp>::StopNetwork(GameObjectSPtr gameobject)
{
    auto comp = gameobject->GetComponent<TServerComp>();
    if (comp == nullptr)
        return false;

    comp->Stop();
    return true;
}

template<class TServerComp>
bool ServerSystem<TServerComp>::AsyncConnect(GameObjectSPtr gameobject, const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect)
{
    auto comp = gameobject->GetComponent<TServerComp>();
    if (comp == nullptr)
        return false;

    return comp->Connect(ip, port, timeout, on_connect);
}

template<class TServerComp>
size_t ServerSystem<TServerComp>::Send(GameObjectSPtr gameobject, bbt::network::ConnId id, const char* bytes, size_t len)
{
    auto comp = gameobject->GetComponent<TServerComp>();
    if (comp == nullptr)
        return false;
    
    return comp->Send(id, bytes, len);
}

template<class TServerComp>
void ServerSystem<TServerComp>::Close(GameObjectSPtr gameobject, bbt::network::ConnId id)
{
    auto comp = gameobject->GetComponent<TServerComp>();
    if (comp == nullptr)
        return;
    
    comp->Close(id);
}

}