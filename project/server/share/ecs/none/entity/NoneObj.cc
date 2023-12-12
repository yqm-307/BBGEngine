#include "share/ecs/none/entity/NoneObj.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::none
{

NoneObj::NoneObj()
    :engine::ecs::GameObject(share::ecs::emEntityType::EM_ENTIRY_TYPE_NONE)
{
    OnCreate();
}

NoneObj::~NoneObj()
{
    OnDestory();
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
