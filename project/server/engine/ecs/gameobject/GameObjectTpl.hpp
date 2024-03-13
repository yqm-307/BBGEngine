#include "engine/ecs/gameobject/GameObject.hpp"

namespace engine::ecs
{

template<class Derived>
bool GameObject::TypeSafeCheck<Derived>()
{
    std::is_base_of_v(std::enable_shared_from_this<GameObject>, Derived);
}

}