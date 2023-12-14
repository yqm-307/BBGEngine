#include "./AoiComponent.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::aoi
{


AoiComponent::AoiComponent(OnEnterFunc onenter, OnLeaveFunc onleave)
    :Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI),
    m_config(G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi)),
    m_gameobj_map([](int key){return key%AoiHashBucketNum;}, nullptr),
    m_comp_name(engine::ecs::ComponentMgr::GetInstance()->GetComponentName(m_comp_template_id)),
    m_enter_func(onenter),
    m_leave_func(onleave),
    m_create_ms(bbt::timer::clock::now())
{
    Init();
    AssertWithInfo(!m_comp_name.empty() , "not found component!");
    AssertWithInfo(CheckConfig(m_config), "aoi config check failed!");
}

AoiComponent::~AoiComponent()
{
}

void AoiComponent::Init()
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

void AoiComponent::OnAddComponent(engine::ecs::GameObjectSPtr component)
{
}

void AoiComponent::OnDelComponent(engine::ecs::GameObjectSPtr component)
{
}

void AoiComponent::OnUpdate()
{   
    auto pass_ms = std::chrono::duration_cast<bbt::timer::clock::ms>(bbt::timer::clock::now() - m_prev_info_ms).count();
    if(pass_ms >= AoiInfoIntervalMS)
    {
        GAME_BASE_LOG_INFO("Aoi: current player num: %d", m_gameobj_map.Size());
        m_prev_info_ms = bbt::timer::clock::now();
    }
}

util::pos::Index3 AoiComponent::GetIndex3ByPos3(util::vector::Vector3 pos3) const
{
    util::pos::Index3 index;
    if( pos3.m_x < 0 || pos3.m_y < 0 || pos3.m_z < 0 )
        return index;
    index.x = (int)(pos3.m_x / m_config->m_tower_x);
    index.y = (int)(pos3.m_y / m_config->m_tower_y);
    index.z = (int)(pos3.m_z / m_config->m_tower_z);
    return index;
}

util::pos::Index3 AoiComponent::GetIndex3ByIndex(int tower_index) const
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

Tower* AoiComponent::GetTowerByPos3(util::vector::Vector3 pos3)
{
    util::pos::Index3 index3 = GetIndex3ByPos3(pos3);
    if( pos3.m_x < 0 || index3.x >= m_tower_max_x ||
        pos3.m_y < 0 || index3.y >= m_tower_max_y ||
        pos3.m_z < 0 || index3.z >= m_tower_max_z )
        return nullptr;
    
    int index = index3.z * (m_tower_max_x * m_tower_max_y) + 
                index3.x * (m_tower_max_y) + 
                index3.y;
    return &(m_towers[index]);
}

Tower* AoiComponent::GetTowerByIndex3(util::pos::Index3 index3)
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

engine::ecs::GameObjectSPtr AoiComponent::GetGameObj(AoiObjectId id)
{
    auto [gameobj, isexist] = m_gameobj_map.Find(id);
    if(!isexist)
        return nullptr;
    
    return gameobj;
}

void AoiComponent::ScanTowerAround(Tower* center_tower, AroundFunc dofunc)
{
    int x = center_tower->m_index_x,
        y = center_tower->m_index_y,
        z = center_tower->m_index_z;
    int n = 0;
    for( int i = x-1; i <= x+1; i++) {
        for( int j = y-1; j <= y+1; j++ ) {
            for( int k = z-1; k <= z+1; k++ ) {
                auto tmp_tower = GetTowerByIndex3({i, j, k});
                if(tmp_tower != nullptr)
                    dofunc(tmp_tower, ++n);
            }
        }
    }
}

bool AoiComponent::CheckConfig(const util::config::AoiConfig* cfg) const
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

std::vector<engine::ecs::GameObjectSPtr> AoiComponent::GetEntitysEx(util::vector::Vector3 pos)
{
    std::vector<engine::ecs::GameObjectSPtr> rlts;
    do{
        auto tower = GetTowerByPos3(pos);
        if(tower == nullptr)
            break;
        ScanTowerAround(tower, [&rlts](Tower* tower, int n){
            for(auto&& entity: tower->m_players)
            {
                if(entity.second != nullptr)
                    rlts.push_back(entity.second);
            }
        });
    }while(0);
    return rlts;
}

}// namepasce game::share::ecs::component