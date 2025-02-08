#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/network/Network.hpp"


namespace share::ecs::network
{

Network::Network():
    engine::ecs::GameObject(EM_ENTITY_TYPE_NETWORK)
{
}

}