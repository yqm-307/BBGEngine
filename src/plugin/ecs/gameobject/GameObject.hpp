#include "engine/ecs/entity/Entity.hpp"


namespace plugin::ecs::gameobject
{

class GameObject:
    public engine::ecs::Entity
{
    GameObjectClassMetaInfo;
public:
    ~GameObject();

private:
    GameObject();

};

}