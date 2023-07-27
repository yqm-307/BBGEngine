#include "testsystem.hpp"


void SetConfig()
{
    auto* aoi = new game::util::config::AoiConfig();
    aoi->m_map_x = 100.0f;
    aoi->m_map_y = 100.0f;
    aoi->m_map_z = 100.0f;
    aoi->m_tower_x = 3.0f;
    aoi->m_tower_y = 3.0f;
    aoi->m_tower_z = 3.0f;
    G_SetConfigPtr(game::util::config::AoiConfig, aoi, game::util::config::Cfg_Aoi);
}