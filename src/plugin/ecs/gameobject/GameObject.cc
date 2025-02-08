#include "./GameObject.hpp"
#include "plugin/ecs/Define.hpp"

namespace share::ecs::gameobject
{

GameObject::GameObject()
    :engine::ecs::GameObject(emEntityType::EM_ENTITY_TYPE_GAMEOBJECT)
{
}

GameObject::~GameObject()
{

}

}