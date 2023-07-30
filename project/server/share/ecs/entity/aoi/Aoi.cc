#include <cmath>
#include "Aoi.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace game::share::ecs::entity::aoi
{

const ecs::ComponentTemplateId Aoi::m_comp_template_id = ecs::ComponentTemplateId::EM_AoiComponent;

std::shared_ptr<Aoi> Aoi::Create(OnEnterFunc onenter, OnLeaveFunc onleave)
{
    return std::make_shared<Aoi>(onenter, onleave);
}


Aoi::Aoi(OnEnterFunc onenter, OnLeaveFunc onleave)
    :GameObject(ecs::GameObjType::Aoi),
    m_config(G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi)),
    m_gameobj_map([](int key){return key%AoiHashBucketNum;}, nullptr),
    m_comp_name(ecs::GetComponentName(ecs::ComponentTemplateId::EM_AoiComponent)),
    m_enter_func(onenter),
    m_leave_func(onleave)
{
    AssertWithInfo(m_comp_name.empty() , "not found component!");
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


bool Aoi::CheckConfig(const util::config::AoiConfig* cfg) const
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
    Assert(player != nullptr);
    auto aoi_comp = GetAoiComponent(player);
    Assert(aoi_comp != nullptr);
    aoi_comp->Clean();
}

void Aoi::OnMove(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnUpdate()
{

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

game::util::pos::Index3 Aoi::GetIndex3ByIndex(int tower_index) const
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

ecs::GameObject::SPtr Aoi::GetGameObj(AoiObjectId id)
{
    auto [gameobj, isexist] = m_gameobj_map.Find(id);
    if(!isexist)
        return nullptr;
    
    return gameobj;
}

bool Aoi::HasAoiComponent(ecs::GameObject::SPtr obj)
{
    if(obj == nullptr)
        return false;
    auto aoi_comp = obj->GetComponent(m_comp_template_id);
    if(aoi_comp == nullptr)
        return false;
    
    return true;
}

std::shared_ptr<ecs::component::AoiComponent> Aoi::GetAoiComponent(ecs::GameObject::SPtr obj)
{
    if(obj == nullptr)
        return nullptr;
    auto comp = obj->GetComponent(m_comp_template_id);
    if(comp == nullptr)
        return nullptr;
    return std::static_pointer_cast<ecs::component::AoiComponent>(comp); 
}

Tower* Aoi::GetTowerByPos3(util::vector::Vector3 pos3)
{
    auto index3 = GetIndex3ByPos3(pos3);
    if( pos3.m_x < 0 || index3.x >= m_tower_max_x ||
        pos3.m_y < 0 || index3.y >= m_tower_max_y ||
        pos3.m_z < 0 || index3.z >= m_tower_max_z )
        return nullptr;
    
    int index = index3.z * (m_tower_max_x * m_tower_max_y) + 
                index3.x * (m_tower_max_y) + 
                index3.y;
    return &(m_towers[index]);
}

util::pos::Index3 Aoi::GetIndex3ByPos3(util::vector::Vector3 pos3) const
{
    util::pos::Index3 index;
    if( pos3.m_x < 0 || pos3.m_y < 0 || pos3.m_z < 0 )
        return index;
    index.x = (int)(pos3.m_x / m_config->m_tower_x);
    index.y = (int)(pos3.m_y / m_config->m_tower_y);
    index.z = (int)(pos3.m_z / m_config->m_tower_z);
    return index;
}

void Aoi::ScanTowerAround(Tower* center_tower, AroundFunc dofunc)
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


#pragma endregion


#pragma region "AOI函数"

void Aoi::EnterAoi(game::share::ecs::GameObject::SPtr player, util::vector::Vector3 drop_point)
{
    std::shared_ptr<ecs::component::AoiComponent> aoi_comp = GetAoiComponent(player);
    if(aoi_comp == nullptr)
        return;
    
    int id = aoi_comp->GetObjId();
    ecs::GameObject::SPtr old_obj = GetGameObj(id);

    if(old_obj != nullptr)
        LeaveAoi(player);
    
    /* 获取灯塔下标 */
    auto tower = GetTowerByPos3(drop_point);
    if(tower == nullptr)
        return;
    /* 移动aoi obj */
    aoi_comp->Moveto(drop_point);
    aoi_comp->Moveto(tower);

    /* 将player加入aoi中管理 */
    [[maybe_unused]] auto isok = m_gameobj_map.Insert(id, player);
    AssertWithInfo(isok, "player is existed in aoi!");
    /* 将player加入到灯塔中 */
    [[maybe_unused]] auto [_,isok2] = tower->m_players.insert(std::make_pair(id, player));
    AssertWithInfo(isok2, "player is existed in tower!");

    /* 通知九宫格所有人 */
    ScanTowerAround(tower, [this, player](Tower* tower, int n){
        EnterTowerBroadCast(player, tower, n);
    });
    OnEnter(player);
}

void Aoi::LeaveAoi(ecs::GameObject::SPtr player)
{
    std::shared_ptr<ecs::component::AoiComponent> aoi_comp = GetAoiComponent(player);
    if(aoi_comp == nullptr)
        return;
    int id = aoi_comp->GetObjId();
    ecs::GameObject::SPtr old_obj = GetGameObj(id);
    
    /* 不存在aoi中直接返回 */
    if(old_obj == nullptr)
        return;
    auto tower = GetTowerByPos3(aoi_comp->GetCurrentPos());
    AssertWithInfo(tower != nullptr, "player pos info failed!");
    
    /* 将player从aoi中删除 */
    auto [_, isok] = m_gameobj_map.Earse(id);
    AssertWithInfo(isok, "aoi remove gameobj failed!");
    
    /* 将player从灯塔中删除 */
    [[maybe_unused]] auto a = tower->m_players.erase(id);
    AssertWithInfo(a > 0, "tower not found player!");

    /* 通知灯塔范围内的所有人 */
    ScanTowerAround(tower, [this, player](Tower* tower, int n){
        LeaveTowerBroadCast(player, tower, n);
    });
    OnLeave(player);
}

void Aoi::Move(ecs::GameObject::SPtr player, util::vector::Vector3 new_pos)
{
    auto aoi_comp = GetAoiComponent(player);
    if(aoi_comp == nullptr)
        return;

    [[maybe_unused]] int id = aoi_comp->GetObjId();
    AssertWithInfo(id >= 0, "aoi object id error!");
    auto old_pos = aoi_comp->GetCurrentPos();
    
    /* 新灯塔插入，旧灯塔删除 */
    auto new_tower = GetTowerByPos3(new_pos);
    auto old_tower = GetTowerByPos3(old_pos);
    AssertWithInfo(RemoveObjFromTowerById(old_tower, id) != nullptr, "game obj not found!");
    AssertWithInfo(InsertObj2Tower(new_tower, id, player), "game obj is existed!");

    /* 移动aoi obj */
    aoi_comp->Moveto(new_pos);
    aoi_comp->Moveto(new_tower);

    /* 通知灯塔范围内的玩家 */
    if(new_tower == old_tower)
        return;
    
    /* 广播新灯塔九宫格 */
    ScanTowerAround(new_tower, [this, player](Tower* tower, int n){
        this->EnterTowerBroadCast(player, tower, n);
    });
    /* 广播旧灯塔九宫格 */
    ScanTowerAround(old_tower, [this, player](Tower* tower, int n){
        this->LeaveTowerBroadCast(player, tower, n);
    });
}

void Aoi::EnterTowerBroadCast(ecs::GameObject::SPtr player, Tower* tower, int n)
{
    /**
     * 当一个实体离开灯塔范围的时候，应该通知之前的灯塔中所有的观察者。
     * 并通知实体自己，这样才能做到实体移动过程中可以被移动前后区域感知
     */
    auto aoi_comp = GetAoiComponent(player);
    if(aoi_comp == nullptr)
        return;

    for (auto it_tplayer : tower->m_players)
    {
        if(it_tplayer.first == aoi_comp->GetObjId())
            return;
            
        auto it_comp = GetAoiComponent(it_tplayer.second);
        if(it_comp == nullptr)
            return;
            
        if( aoi_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_enter_func(player, it_tplayer.second);
            
        if( it_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_enter_func(it_tplayer.second, player);
    }
}

void Aoi::LeaveTowerBroadCast(ecs::GameObject::SPtr player, Tower* tower, int n)
{
    /**
     * 当一个实体离开灯塔范围的时候，应该通知之前的灯塔中所有的观察者。
     * 并通知实体自己，这样才能做到实体移动过程中可以被移动前后区域感知
     */
    auto aoi_comp = GetAoiComponent(player);
    if(aoi_comp == nullptr)
        return;
        
    for (auto it_tplayer : tower->m_players)
    {
        if(it_tplayer.first == aoi_comp->GetObjId())
            return;
            
        auto it_comp = GetAoiComponent(it_tplayer.second);
        if(it_comp == nullptr)
            return;
            
        if( aoi_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_leave_func(player, it_tplayer.second);

        if( it_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_leave_func(it_tplayer.second, player);
    }
}

ecs::GameObject::SPtr Aoi::RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id)
{
    if(from_tower == nullptr || from_tower->m_players.empty())
        return nullptr;
        
    auto it = from_tower->m_players.find(id);
    if(it == from_tower->m_players.end())
        return nullptr;
    
    from_tower->m_players.erase(it);    
    return it->second;
}

bool Aoi::InsertObj2Tower(Tower* to_tower, AoiObjectId id, ecs::GameObject::SPtr obj)
{
    if(to_tower == nullptr || id < 0 || obj == nullptr)
        return false;

    /* debug阶段做好，我们可以保证runtime不会出问题。这样就不需要多次取id */
#ifdef Debug
    auto aoi_comp = GetAoiComponent(obj);
    AssertWithInfo(aoi_comp != nullptr, "gameobj has no AoiComponent!");
    AssertWithInfo(aoi_comp->GetObjId() == id, "aoi id not equal!");
#endif

    auto it = to_tower->m_players.find(id);
    if(it != to_tower->m_players.end())
        return false;
    
    to_tower->m_players.insert(std::make_pair(id, obj));
    return true;
}


#pragma endregion

}