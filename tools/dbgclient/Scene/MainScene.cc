#include "MainScene.hpp"
#include "plugin/ecs/entity/Entity.hpp"
#include "plugin/ecs/network/NetworkComponent.hpp"

namespace dbgclient::scene
{

MainScene::~MainScene()
{

}

MainScene::MainScene(const char* ip, short port)
{
    auto network_obj  = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    auto network_comp = G_ComponentMgr()->Create<plugin::ecs::network::NetworkComponent>();

    // MountGameObject();
}

}