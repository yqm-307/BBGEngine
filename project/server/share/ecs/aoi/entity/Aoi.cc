#include <cmath>
#include "share/ecs/aoi/entity/Aoi.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"
#include "engine/ecs/component/ComponentMgr.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::aoi
{


Aoi::Aoi()
    :GameObject(share::ecs::EM_ENTITY_TYPE_AOI)
{
}

Aoi::~Aoi()
{

}

bool Aoi::CheckConfig(const util::config::AoiConfig* cfg) const
{
    AssertWithInfo(cfg != nullptr, " config not found!");
    AssertWithInfo(
        cfg->m_tower_x > 0 && 
        cfg->m_tower_y > 0 && 
        cfg->m_tower_z > 0 &&
        cfg->m_map_x >= cfg->m_tower_x && 
        cfg->m_map_y >= cfg->m_tower_y && 
        cfg->m_map_z >= cfg->m_tower_z,
        "aoi config invalid!");
    return true;
}

void Aoi::OnUpdate()
{
}

} // namespace engine::ecs::entity::aoi
