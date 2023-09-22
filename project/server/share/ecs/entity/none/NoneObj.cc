#include "share/ecs/entity/none/NoneObj.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::none
{

NoneObj::NoneObj()
    :engine::ecs::GameObject(share::ecs::emEntityType::EM_ENTIRY_TYPE_NONE)
{
    
}

NoneObj::~NoneObj()
{

}

void NoneObj::OnCreate()
{

}

void NoneObj::OnDestory()
{

}

void NoneObj::OnUpdate()
{

}

} // namespace share::ecs::entity::none
