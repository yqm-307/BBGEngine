#include "engine/ecs/gameobject/GameObject.hpp"


namespace share::ecs::gameobject
{

class GameObject:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    ~GameObject();

private:
    GameObject();

};

}