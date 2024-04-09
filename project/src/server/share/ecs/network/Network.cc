#include "share/ecs/Define.hpp"
#include "share/ecs/network/Network.hpp"


namespace share::ecs::network
{

Network::Network():
    engine::ecs::GameObject(EM_ENTITY_TYPE_NETWORK)
{
}

}