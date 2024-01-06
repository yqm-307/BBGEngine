#include "share/ecs/none/entity/NoneObj.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::noneobj
{

NoneObj::NoneObj()
    :engine::ecs::GameObject(share::ecs::emEntityType::EM_ENTITY_TYPE_GAMEOBJECT)
{
}

NoneObj::~NoneObj()
{
}


} // namespace share::ecs::entity::none
