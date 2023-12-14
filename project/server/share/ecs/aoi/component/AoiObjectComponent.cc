#include "AoiObjectComponent.hpp"

namespace share::ecs::aoi
{

AoiObjectComponent::AoiObjectComponent()
    :Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT)
{
}

AoiObjectComponent::~AoiObjectComponent()
{
}

ecs::aoi::AoiEntityFlag AoiObjectComponent::GetMode()
{
    return m_mode;
}

void AoiObjectComponent::OnMove(util::vector::Vector3 vec)
{

}

void AoiObjectComponent::OnMove(Tower* tower)
{
}

util::vector::Vector3 AoiObjectComponent::GetCurrentPos()
{
    return m_curr_pos;
}

AoiObjectId AoiObjectComponent::GetObjId()
{
    return m_id;
}



}