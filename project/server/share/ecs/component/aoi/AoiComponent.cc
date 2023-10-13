#include "./AoiComponent.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::component
{


AoiComponent::AoiComponent()
    :Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI),
    m_aoi_object_id(-1),
    m_aoi_entity_mode(-1)
{
}

AoiComponent::~AoiComponent()
{
}

void AoiComponent::OnAddComponent(engine::ecs::GameObjectSPtr component)
{
}

void AoiComponent::OnDelComponent(engine::ecs::GameObjectSPtr component)
{
}

void AoiComponent::OnUpdate()
{   
}

int AoiComponent::GetObjId() const
{
    return m_aoi_object_id;
}

void AoiComponent::SetObjId(int id)
{
    DebugAssert(id);
    m_aoi_object_id = id;
}


int AoiComponent::GetEntityMode() const
{
    return m_aoi_entity_mode;
}

util::vector::Vector3 AoiComponent::GetCurrentPos() const
{
    return m_current_pos;
}

void AoiComponent::Moveto(util::vector::Vector3 new_pos)
{
    m_prev_pos = m_current_pos;
    m_current_pos = new_pos;
}

void AoiComponent::Moveto(entity::aoi::Tower* new_tower)
{
    DebugAssert(new_tower != nullptr);
    m_tower = new_tower;
}

entity::aoi::Tower* AoiComponent::GetTower() const
{
    return m_tower;
}

void AoiComponent::Clean()
{
    m_aoi_object_id = -1;
    m_aoi_entity_mode = -1;
    m_tower = nullptr;

    m_aabb_box.max_x = -1.0f;
    m_aabb_box.max_y = -1.0f;
    m_aabb_box.max_z = -1.0f;
    m_aabb_box.min_x = -1.0f;
    m_aabb_box.min_y = -1.0f;
    m_aabb_box.min_z = -1.0f;
    
    m_prev_pos.m_x = -1.0f;
    m_prev_pos.m_y = -1.0f;
    m_prev_pos.m_z = -1.0f;

    m_current_pos.m_x = -1.0f;
    m_current_pos.m_y = -1.0f;
    m_current_pos.m_z = -1.0f;
}


void AoiComponent::Debug_PosChange()
{
    printf("[%.2f , %.2f , %.2f] ===> [%.2f , %.2f , %.2f]\n", 
        m_prev_pos.m_x, m_prev_pos.m_y, m_prev_pos.m_z,
        m_current_pos.m_x, m_current_pos.m_y, m_current_pos.m_z);
}

}// namepasce game::share::ecs::component