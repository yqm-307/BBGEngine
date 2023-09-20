#pragma once
#include <cstddef>
#include <set>
#include <map>
#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/vector/Vector2.hpp"

namespace game::share::ecs::entity::aoi
{

typedef float       Pos;            // 
typedef int         AoiObjectId;    // aoi 对象id, aoi中任何 gameobject 都有该id
typedef int         Index;          // 

typedef std::map<int, share::ecs::GameObjectSPtr> GameObjMap;


/**
 * 灯塔：在aoi中即为关注连接，在灯塔范围内的处于一个aoi区域
 */
struct Tower
{
    /* 灯塔照亮的三维区域 */
    float      m_index_x{-1.0f};
    float      m_index_y{-1.0f};
    float      m_index_z{-1.0f};
    /* 灯塔范围内的对象 */
    GameObjMap  m_players;
    /* 此灯塔的关注者 */
    GameObjMap  m_watchers;
};

/* 地图格子 */
struct MapSlot:
    public bbt::templateutil::BaseType<MapSlot>
{
    Pos m_pos_x{-1.0f};
    Pos m_pos_y{-1.0f};
    Pos m_pos_z{-1.0f};
    Tower* m_tower{nullptr};
};

struct AABBBox
{
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
};

static inline bool Overlap(float amax, float amin, float bmax, float bmin)
{
    return !(amax <= bmin || bmax <= amin);
}

static inline bool AABBParamCheck(const AABBBox& a)
{
    return ( bbt_likely(a.max_x > a.min_x) && bbt_likely(a.max_y > a.min_y) && bbt_likely(a.max_z > a.min_z) );
}

static inline bool AABBCheck(const AABBBox& a, const AABBBox& b)
{
    AssertWithInfo(AABBParamCheck(a) , "params error!");
    AssertWithInfo(AABBParamCheck(b) , "params error!");
    bool xover = Overlap(a.max_x, a.min_x, b.max_x, b.min_x);
    bool yover = Overlap(a.max_y, a.min_y, b.max_y, b.min_y);
    bool zover = Overlap(a.max_z, a.min_z, b.max_z, b.min_z);
    return xover && yover && zover;
}

static inline void AoiDefaultOnLeave(ecs::GameObjectSPtr )
{

}

}