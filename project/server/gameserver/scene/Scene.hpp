#pragma once
#include "share/scene/Scene.hpp"

namespace server::scene
{

class GameServerScene:
    public game::share::scene::Scene
{
public:
    GameServerScene();
    ~GameServerScene();

    virtual void OnUpdate() override;
private:

};

}