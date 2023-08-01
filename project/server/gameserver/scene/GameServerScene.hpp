#pragma once
#include "share/scene/Scene.hpp"

namespace server::scene
{

const int GameSceneFrame = 20;  // 服务端游戏场景每秒20帧

class GameServerScene:
    public game::share::scene::Scene
{
public:
    GameServerScene();
    ~GameServerScene();

    virtual void OnUpdate() override;
private:
    std::map<std::string, game::share::ecs::GameObjectSPtr> m_all_obj;
};

}