#include "engine/ecs/gameobject/GameObject.hpp"


namespace plugin::ecs::gameobject
{

class GameObject:
    public engine::ecs::GameObject
{
    GameObjectClassMetaInfo;
public:
    ~GameObject();

private:
    GameObject();

};

}