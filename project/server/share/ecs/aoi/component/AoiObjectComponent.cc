#include "AoiObjectComponent.hpp"
#include "util/vector/Vector3.hpp"

namespace share::ecs::aoi
{

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


}