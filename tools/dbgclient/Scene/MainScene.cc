#include "MainScene.hpp"
#include "plugin/ecs/gameobject/GameObject.hpp"
#include "plugin/ecs/network/NetworkComponent.hpp"

namespace dbgclient::scene
{

MainScene::~MainScene()
{

}

MainScene::MainScene(const char* ip, short port)
{
    auto network_obj  = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto network_comp = G_ComponentMgr()->Create<share::ecs::network::NetworkComponent>();

    // MountGameObject();
}

}