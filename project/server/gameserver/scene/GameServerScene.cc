#include "gameserver/scene/GameServerScene.hpp"


namespace server::scene
{

GameServerScene::GameServerScene()
{
}

GameServerScene::~GameServerScene()
{
}

void GameServerScene::OnUpdate()
{
    for(auto&& obj : m_all_obj)
    {
        obj.second->OnUpdate();
    }
}


} 