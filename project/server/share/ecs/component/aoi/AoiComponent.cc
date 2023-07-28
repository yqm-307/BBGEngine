#include "AoiComponent.hpp"

namespace game::share::ecs::component
{


AoiComponent::AoiComponent()
    :Component("AoiComponent")
{

}

AoiComponent::~AoiComponent()
{

}

void AoiComponent::OnAddComponent()
{

}

void AoiComponent::OnDelComponent()
{

}

void AoiComponent::OnUpdate()
{
    
}

int AoiComponent::GetObjId() const
{
    return m_aoi_object_id;
}

int AoiComponent::GetEntityMode() const
{
    return m_aoi_entity_mode;
}

util::vector::Vector3 AoiComponent::GetCurrentPos() const
{
    return m_current_pos;
}



}// namepasce game::share::ecs::component