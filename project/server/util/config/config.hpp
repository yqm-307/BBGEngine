#pragma once
#include "configutil.hpp"

namespace game::util::config
{

struct AoiConfig
{
    size_t m_map_x;
    size_t m_map_y;
    size_t m_map_z;

    size_t m_tower_x;
    size_t m_tower_y;
    size_t m_tower_z;
};



}