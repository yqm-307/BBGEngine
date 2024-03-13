#include "./GameObject.hpp"
#include "share/ecs/Define.hpp"

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