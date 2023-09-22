#include <cmath>
#include "share/ecs/entity/aoi/Aoi.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"
#include "engine/ecs/component/ComponentMgr.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::aoi
{

const engine::ecs::ComponentTemplateId Aoi::m_comp_template_id = engine::ecs::ComponentTemplateId::EM_AoiComponent;

std::shared_ptr<Aoi> Aoi::Create(OnEnterFunc onenter, OnLeaveFunc onleave)
{
    return std::make_shared<Aoi>(onenter, onleave);
}


Aoi::Aoi(OnEnterFunc onenter, OnLeaveFunc onleave)
    :GameObject(share::ecs::EM_ENTITY_TYPE_AOI),
    m_config(G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi)),
    m_gameobj_map([](int key){return key%AoiHashBucketNum;}, nullptr),
    m_comp_name(engine::ecs::ComponentMgr::GetInstance()->GetComponentName(engine::ecs::ComponentTemplateId::EM_AoiComponent)),
    m_enter_func(onenter),
    m_leave_func(onleave),
    m_create_ms(bbt::timer::clock::now())
{
    AssertWithInfo(!m_comp_name.empty() , "not found component!");
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
        cfg->m_map_x >= cfg->m_tower_x && 
        cfg->m_map_y >= cfg->m_tower_y && 
        cfg->m_map_z >= cfg->m_tower_z,
        "aoi config invalid!");
    return true;
}

void Aoi::OnEnter(engine::ecs::GameObjectSPtr player)
{
    // Log();
}

void Aoi::OnLeave(engine::ecs::GameObjectSPtr player)
{
    DebugAssert(player != nullptr);
    auto aoi_comp = GetAoiComponent(player);
    DebugAssert(aoi_comp != nullptr);
    aoi_comp->Clean();
}

void Aoi::OnMove(engine::ecs::GameObjectSPtr player)
{

}

void Aoi::OnUpdate()
{
    auto pass_ms = std::chrono::duration_cast<bbt::timer::clock::ms>(bbt::timer::clock::now() - m_prev_info_ms).count();
    if(pass_ms >= AoiInfoIntervalMS)
    {
        GAME_BASE_LOG_INFO("Aoi: current player num: %d", m_gameobj_map.Size());
        m_prev_info_ms = bbt::timer::clock::now();
    }
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

util::pos::Index3 Aoi::GetIndex3ByIndex(int tower_index) const
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

engine::ecs::GameObjectSPtr Aoi::GetGameObj(AoiObjectId id)
{
    auto [gameobj, isexist] = m_gameobj_map.Find(id);
    if(!isexist)
        return nullptr;
    
    return gameobj;
}

bool Aoi::HasAoiComponent(engine::ecs::GameObjectSPtr obj)
{
    if(obj == nullptr)
        return false;
    auto aoi_comp = obj->GetComponent(m_comp_template_id);
    if(aoi_comp == nullptr)
        return false;
    
    return true;
}

std::shared_ptr<ecs::component::AoiComponent> Aoi::GetAoiComponent(engine::ecs::GameObjectSPtr obj)
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



void Aoi::EnterTowerBroadCast(engine::ecs::GameObjectSPtr player, Tower* tower, int n)
{
    /**
     * 当一个实体离开灯塔范围的时候，应该通知之前的灯塔中所有的观察者。
     * 并通知实体自己，这样才能做到实体移动过程中可以被移动前后区域感知
     */
    auto aoi_comp = GetAoiComponent(player);
    if(bbt_unlikely(aoi_comp == nullptr))
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

void Aoi::LeaveTowerBroadCast(engine::ecs::GameObjectSPtr player, Tower* tower, int n)
{
    /**
     * 当一个实体离开灯塔范围的时候，应该通知之前的灯塔中所有的观察者。
     * 并通知实体自己，这样才能做到实体移动过程中可以被移动前后区域感知
     */
    auto aoi_comp = GetAoiComponent(player);
    if(bbt_unlikely(aoi_comp == nullptr))
        return;
        
    for (auto it_tplayer : tower->m_players)
    {
        /* 跳过自己？是否通知自己呢，感觉还是需要通知自己的 */
        // if( it_tplayer.first == aoi_comp->GetObjId())
        //     return;
            
        auto it_comp = GetAoiComponent(it_tplayer.second);
        if(bbt_unlikely(it_comp == nullptr))
            return;
            
        if( aoi_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_leave_func(player, it_tplayer.second);

        if( it_comp->GetEntityMode() & ecs::entity::aoi::AoiEntityFlag::Watcher )
            m_leave_func(it_tplayer.second, player);
    }
}

engine::ecs::GameObjectSPtr Aoi::RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id)
{
    if(from_tower == nullptr || from_tower->m_players.empty())
        return nullptr;
        
    auto it = from_tower->m_players.find(id);
    if(it == from_tower->m_players.end())
        return nullptr;
    
    from_tower->m_players.erase(it);    
    return it->second;
}

bool Aoi::InsertObj2Tower(Tower* to_tower, AoiObjectId id, engine::ecs::GameObjectSPtr obj)
{
    if(to_tower == nullptr || id < 0 || obj == nullptr)
        return false;

#ifdef Debug
    /* debug阶段做好，我们可以保证runtime不会出问题。这样就不需要多次取id */
    auto aoi_comp = GetAoiComponent(obj);
    DebugAssertWithInfo(aoi_comp != nullptr, "gameobj has no AoiComponent!");
    DebugAssertWithInfo(aoi_comp->GetObjId() == id, "aoi id not equal!");
#endif

    auto it = to_tower->m_players.find(id);
    if(it != to_tower->m_players.end())
        return false;
    
    to_tower->m_players.insert(std::make_pair(id, obj));
    return true;
}

std::vector<engine::ecs::GameObjectSPtr>  Aoi::GetEntitysEx(util::vector::Vector3 pos)
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

#pragma endregion


#pragma region "Aoi公共接口"

bool Aoi::EnterAoi(engine::ecs::GameObjectSPtr player, util::vector::Vector3 drop_point)
{
    bool success = false;
    do
    {
        std::shared_ptr<ecs::component::AoiComponent> aoi_comp = GetAoiComponent(player);
        if(aoi_comp == nullptr)
            break;

        int id = aoi_comp->GetObjId();
        if(id < 0)
            break;

        engine::ecs::GameObjectSPtr old_obj = GetGameObj(id);
        if(bbt_unlikely(old_obj == nullptr))
            LeaveAoi(player);
        
        /* 获取灯塔下标 */
        auto tower = GetTowerByPos3(drop_point);
        if(bbt_unlikely(tower == nullptr))
            break;

        /* 移动aoi obj */
        aoi_comp->Moveto(drop_point);
        aoi_comp->Moveto(tower);

        /* 将player加入aoi中管理 */
        auto isok = m_gameobj_map.Insert(id, player);
        if(bbt_unlikely(!isok))
            break;

        /* 将player加入到灯塔中 */
        auto [_,isok2] = tower->m_players.insert(std::make_pair(id, player));
        if(bbt_unlikely(!isok2))
            break;

        success = true;
        /* 通知九宫格所有人 */
        ScanTowerAround(tower, [this, player](Tower* tower, int n){
            EnterTowerBroadCast(player, tower, n);
        });
        OnEnter(player);
    }while(0);
    
    return success;
}

bool Aoi::LeaveAoi(engine::ecs::GameObjectSPtr player)
{
    bool success = false;
    do
    {
        std::shared_ptr<ecs::component::AoiComponent> aoi_comp = GetAoiComponent(player);
        if(bbt_unlikely(aoi_comp == nullptr))
            break;

        int id = aoi_comp->GetObjId();
        if(bbt_unlikely(id < 0))
            break;

        engine::ecs::GameObjectSPtr old_obj = GetGameObj(id);
        if(bbt_unlikely(old_obj == nullptr))
            break;

        auto tower = GetTowerByPos3(aoi_comp->GetCurrentPos());
        if(bbt_unlikely(tower == nullptr))
            break;
        
        /* 将player从aoi中删除 */
        auto [_, isok] = m_gameobj_map.Earse(id);
        if(bbt_unlikely(!isok))
            break;
        
        /* 将player从灯塔中删除 */
        auto a = RemoveObjFromTowerById(tower, id);
        if(bbt_unlikely(a == nullptr))
            break;            

        success = true;
        /* 通知灯塔范围内的所有人 */
        ScanTowerAround(tower, [this, player](Tower* tower, int n){
            LeaveTowerBroadCast(player, tower, n);
        });
        OnLeave(player);
    }while(0);

    return success;
}

bool Aoi::Move(engine::ecs::GameObjectSPtr player, util::vector::Vector3 new_pos)
{
    bool success = false;
    do
    {
        /* 是否可以看做aoi中的对象 */
        auto aoi_comp = GetAoiComponent(player);
        if(bbt_unlikely(aoi_comp == nullptr))
            break;

        /* id是否合法 */
        int id = aoi_comp->GetObjId();
        if(bbt_unlikely(id < 0))
            break;

        auto old_pos = aoi_comp->GetCurrentPos();        
        /* 是否为aoi中对象，debug检测 */
        DebugAssertWithInfo(GetGameObj(id) != nullptr , "aoi object not found");

        /* 新灯塔插入，旧灯塔删除 */
        auto new_tower = GetTowerByPos3(new_pos);
        auto old_tower = GetTowerByPos3(old_pos);

        /* 从旧灯塔中删除 */
        auto oldobj = RemoveObjFromTowerById(old_tower, id);
        if(bbt_unlikely(oldobj == nullptr))
            break;
        
        /* 插入到新灯塔中 */
        bool isexist = InsertObj2Tower(new_tower, id, player);
        if(bbt_unlikely(!isexist))
            break;

        /* 移动aoi obj */
        aoi_comp->Moveto(new_pos);
        aoi_comp->Moveto(new_tower);

        /* 通知灯塔范围内的玩家 */
        if(bbt_unlikely(new_tower == old_tower))
            break;

        success = true;
        /* 广播新灯塔九宫格 */
        ScanTowerAround(new_tower, [this, player](Tower* tower, int n){
            this->EnterTowerBroadCast(player, tower, n);
        });
        /* 广播旧灯塔九宫格 */
        ScanTowerAround(old_tower, [this, player](Tower* tower, int n){
            this->LeaveTowerBroadCast(player, tower, n);
        });
    }while(0);
    
    return success;
}

Aoi::EntityResult Aoi::GetEntitysByPos(util::vector::Vector3 pos)
{
    std::vector<engine::ecs::GameObjectSPtr> rlts;
    rlts = GetEntitysEx(pos);
    return rlts;
}

Aoi::EntityResult Aoi::GetEntitysByGameobj(engine::ecs::GameObjectSPtr gameobj)
{
    std::vector<engine::ecs::GameObjectSPtr> rlts;
    do{
        if(gameobj == nullptr)
            break;

        auto aoi_comp = GetAoiComponent(gameobj);
        if(aoi_comp == nullptr)
            break;

        auto id = aoi_comp->GetObjId();
        if(bbt_unlikely(id < 0))
            break;
        
        /* debug模式严格审查，release减少计算，不再次取值 */
        DebugAssert(GetGameObj(id) != nullptr);
        
        auto pos = aoi_comp->GetCurrentPos();
        if(util::vector::Vec3IsNull(pos))
            break;
        
        rlts = GetEntitysByPos(pos);
    }while(0);
    
    return rlts;
}

Aoi::EntityResult Aoi::GetEntitysByAoiObjId(AoiObjectId aoiobj_id)
{
    std::vector<engine::ecs::GameObjectSPtr> rlts;
    do{ 
        if(bbt_unlikely(aoiobj_id < 0))
            break;

        DebugAssert(GetGameObj(aoiobj_id) != nullptr);
        
        auto gobj = GetEntityByAoiObjectId(aoiobj_id);
        if(gobj == nullptr)
            break;
        
        auto aoi_comp = GetAoiComponent(gobj);
        if(aoi_comp == nullptr)
            break;
        
        auto pos = aoi_comp->GetCurrentPos();
        if(util::vector::Vec3IsNull(pos))
            break;

        rlts = GetEntitysByPos(pos);
    }while(0);
    return rlts;
}

bool Aoi::CheckEntityIsInAoi(AoiObjectId aoiobj_id)
{
    auto [_, isexist] = m_gameobj_map.Find(aoiobj_id);
    return isexist;
}

engine::ecs::GameObjectSPtr Aoi::GetEntityByAoiObjectId(AoiObjectId aoiobj_id)
{
    if(aoiobj_id < 0)
        return nullptr;

    auto [entity, isexist] = m_gameobj_map.Find(aoiobj_id);
    if(bbt_unlikely(!isexist))
        return nullptr;

    return entity;
}


#pragma endregion


} // namespace engine::ecs::entity::aoi
