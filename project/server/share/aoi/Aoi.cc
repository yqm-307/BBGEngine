#include "share/aoi/Aoi.hpp"
#include "util/log/log.hpp"
#include "util/assert/Assert.hpp"

using namespace game::share::aoi;


Aoi::RawPtr Aoi::GetInstance()
{
    static RawPtr instance = nullptr;
    if(instance == nullptr)
    {
        instance = new Aoi();
    }
    return instance;
}

Aoi::Aoi()
    :m_config(G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi))
{
    CheckConfig(m_config);
    Init();
}

Aoi::~Aoi()
{

}

void Aoi::Init()
{
    auto m_slot_num = m_config->m_map_x * m_config->m_map_y * m_config->m_map_z;
    // m_slots = new MapSlot[m_slot_num];
}

bool Aoi::CheckConfig(const util::config::AoiConfig* cfg)
{
    AssertWithInfo(cfg == nullptr, " config not found!");
    AssertWithInfo(
        cfg->m_tower_x > 0 && 
        cfg->m_tower_y > 0 && 
        cfg->m_tower_z > 0 &&
        cfg->m_map_x > cfg->m_tower_x && 
        cfg->m_map_y > cfg->m_tower_y && 
        cfg->m_map_z > cfg->m_tower_z,
        "aoi config invalid!");

}

void Aoi::OnEnter(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnLeave(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnMove(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnUpdate(game::share::ecs::GameObject::SPtr player)
{

}