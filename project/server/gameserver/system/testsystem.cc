#include "testsystem.hpp"


void SetConfig()
{
    auto* aoi = new game::util::config::AoiConfig();
    aoi->m_map_x = 1.1f;
    aoi->m_map_y = 1.2f;
    aoi->m_map_z = 2.2f;
    aoi->m_tower_x = 3.0f;
    G_SetConfigPtr(game::util::config::AoiConfig, aoi, game::util::config::Cfg_Aoi);
}