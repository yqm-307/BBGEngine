#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>
#include <engine/ecs/system/System.hpp>

namespace share::ecs::network
{

class DBServiceConnObj:
    public engine::ecs::GameObject
{
public:
    virtual ~DBServiceConnObj();
protected:
    DBServiceConnObj();
    virtual void OnUpdate() override;
private:
};


}