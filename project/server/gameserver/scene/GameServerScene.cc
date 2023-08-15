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
    DebugAssert(module_aoi != nullptr);
    module_aoi->OnUpdate();  // 更新aoi
}


} 