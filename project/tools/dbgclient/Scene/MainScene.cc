#include "MainScene.hpp"
#include "share/ecs/gameobject/GameObject.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

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