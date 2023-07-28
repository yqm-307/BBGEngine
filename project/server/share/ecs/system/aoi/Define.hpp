#pragma once
#include <cstddef>
#include <set>
#include <map>
#include "share/ecs/GameObject.hpp"

namespace game::share::ecs::system
{

typedef float       Pos;            // 
typedef int         AoiObjectId;    // aoi 对象id, aoi中任何 gameobject 都有该id
typedef int         Index;          // 

typedef std::map<int, share::ecs::GameObject::SPtr> GameObjMap;


/**
 * 灯塔：在aoi中即为关注连接，在灯塔范围内的处于一个aoi区域
 */
struct Tower
{
    /* 灯塔照亮的三维区域 */
    float      m_index_x;
    float      m_index_y;
    float      m_index_z;
    /* 灯塔范围内的对象 */
    GameObjMap  m_players;
    /* 此灯塔的关注者 */
    GameObjMap  m_watchers;
};

/* 地图格子 */
struct MapSlot:
    public bbt::templateutil::BaseType<MapSlot>
{
    Pos m_pos_x;
    Pos m_pos_y;
    Pos m_pos_z;
    Tower* m_tower;
};

struct AABBBox:
    public bbt::templateutil::BaseType<AABBBox>
{
    float m_len_x;
    float m_len_y;
    float m_len_z;
};


}