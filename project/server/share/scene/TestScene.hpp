#pragma once
#include "engine/scene/Scene.hpp"

namespace share::scene
{

class TestScene:
    public engine::scene::Scene
{
public:

private:
    engine::ecs::GameObjectSPtr m_root_obj;
};

};