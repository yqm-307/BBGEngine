#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>
#include <engine/ecs/system/System.hpp>

namespace plugin::ecs::network
{

class DBServiceConnObj:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    virtual ~DBServiceConnObj();
protected:
    DBServiceConnObj();
    virtual void OnUpdate() override;
private:
};


}