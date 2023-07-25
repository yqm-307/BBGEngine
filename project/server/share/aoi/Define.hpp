#pragma once
#include <cstddef>
#include <set>
#include "share/ecs/GameObject.hpp"

namespace game::share::aoi
{

typedef size_t Pos;

struct GameObjSet
{
    std::set<share::ecs::GameObject::SPtr> m_set; 
};


/**
 * 灯塔：在aoi中即为关注连接，在灯塔范围内的处于一个aoi区域
 */
struct Tower
{
    /* 灯塔照亮的三维区域 */
    float      m_max_len_x;
    float      m_max_len_y;
    float      m_max_len_z;
    /* 灯塔范围内的对象 */
    GameObjSet  m_players;
    /* 此灯塔的关注者 */
    GameObjSet  m_watchers;
};

/* 地图格子 */
struct MapSlot
{
    Pos m_pos_x;
    Pos m_pos_y;
    Pos m_pos_z;
    Tower* m_tower;
};

struct AABBBox
{
    float m_len_x;
    float m_len_y;
    float m_len_z;
};


}