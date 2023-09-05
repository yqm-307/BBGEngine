#pragma once
#include "util/config/configutil.hpp"

namespace game::util::config
{

struct AoiConfig
{
    float m_map_x;
    float m_map_y;
    float m_map_z;

    float m_tower_x;
    float m_tower_y;
    float m_tower_z;
};

struct evConnMgrConfig
{
    int m_heart_beat_once_ms;       // 多久检测一次心跳
    int m_heart_beat_timeout_ms;    // 心跳的超时时间是多久
};

// struct 
// {
//     /* data */
// };


}