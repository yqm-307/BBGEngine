#include "share/ecs/Define.hpp"
#include "share/ecs/network/Network.hpp"


namespace share::ecs::network
{

Network::Network():
    engine::ecs::GameObject(EM_ENTITY_TYPE_NETWORK)
{
    auto network_comp = G_ComponentMgr()->Create<share::ecs::network::NetworkComponent>();
    Assert(AddComponent(network_comp));
    auto connmgr_comp = G_ComponentMgr()->Create<share::ecs::network::ConnMgr>();
    Assert(AddComponent(connmgr_comp));
}

}