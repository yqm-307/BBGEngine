#include <cmath>
#include "share/aoi/Aoi.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace game::share::aoi
{



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
    :m_config(G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi)),
    m_gameobj_map([](int key){return key%AoiHashBucketNum;}, nullptr)
{
    CheckConfig(m_config);
    Init();
}

Aoi::~Aoi()
{

}

void Aoi::Init()
{
    /* 灯塔数量 */
    m_tower_max_x = (int)ceil(m_config->m_map_x / m_config->m_tower_x);
    m_tower_max_y = (int)ceil(m_config->m_map_y / m_config->m_tower_y);
    m_tower_max_z = (int)ceil(m_config->m_map_z / m_config->m_tower_z);
    /* 计算灯塔数量 */
    int tower_total_size = m_tower_max_x * m_tower_max_y * m_tower_max_z;
    m_towers = std::vector<Tower>(tower_total_size);
    /* 初始化灯塔 */
    for(int i = 0; i < m_towers.size(); i++)
    {
        auto [x, y, z] = GetIndex3ByIndex(i);
        m_towers[i].m_index_x = x;
        m_towers[i].m_index_y = y;
        m_towers[i].m_index_z = z;
    }
}


bool Aoi::CheckConfig(const util::config::AoiConfig* cfg)
{
    AssertWithInfo(cfg != nullptr, " config not found!");
    AssertWithInfo(
        cfg->m_tower_x > 0 && 
        cfg->m_tower_y > 0 && 
        cfg->m_tower_z > 0 &&
        cfg->m_map_x > cfg->m_tower_x && 
        cfg->m_map_y > cfg->m_tower_y && 
        cfg->m_map_z > cfg->m_tower_z,
        "aoi config invalid!");
    return true;
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

void Aoi::EnterAoi(game::share::ecs::GameObject::SPtr player, util::pos::Index3 drop_point)
{
    /* 玩家是否已经在Aoi中 */
    // std::dynamic_pointer_cast<ecs::component::>
    // player->GetComponent();
    // ecs::GameObject::SPtr old_obj = GetObjFromAoiById();
}



#pragma region "AOI相关辅助函数实现"

Tower* Aoi::GetTowerByIndex3(util::pos::Index3 index3)
{
    int x=index3.x, y=index3.y, z=index3.z;

    if( x >= m_tower_max_x || x < 0 ||
        y >= m_tower_max_y || y < 0 ||  
        z >= m_tower_max_z || z < 0)
    {
        return nullptr;
    }
    int index = z * m_tower_max_x * m_tower_max_y + /* 在z层之上, z * 每层tower个数  */
                x * m_tower_max_y +                 /* 在x排之后, x * 每排tower个数 */
                y;                                  /* 在y个之后, y */
    return &(m_towers[index]);
}

game::util::pos::Index3 Aoi::GetIndex3ByIndex(int tower_index)
{
    if( tower_index < 0 || tower_index >= m_towers.size() )
        return {-1, -1, -1};
    int x,y,z;
    z = tower_index / (m_tower_max_x * m_tower_max_y);
    tower_index %= (m_tower_max_x * m_tower_max_y);
    x = tower_index / m_tower_max_y;
    tower_index %= m_tower_max_y;
    y = tower_index;
    return {x, y, z};
}

ecs::GameObject::SPtr Aoi::GetObjFromAoiById(AoiObjectId id)
{
    auto [gameobj, isexist] = m_gameobj_map.Find(id);
    if(!isexist)
        return nullptr;
    
    return gameobj;
}

#pragma endregion

}