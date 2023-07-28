#include "AoiComponent.hpp"

using namespace game::share::ecs::component;

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