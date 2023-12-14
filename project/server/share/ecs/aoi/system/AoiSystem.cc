#include <memory>
#include "AoiSystem.hpp"
#include "engine/ecs/component/ComponentMgr.hpp"
#include "share/ecs/aoi/entity/Aoi.hpp"
#include "share/ecs/aoi/component/AoiComponent.hpp"
#include "share/ecs/Define.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace share::ecs::aoi
{
std::unique_ptr<AoiSystem> AoiSystem::GetInstance()
{
    std::unique_ptr<AoiSystem> _inst = nullptr;
    if (_inst == nullptr) {
        _inst = std::unique_ptr<AoiSystem>(new AoiSystem());
    }

    return _inst;
}

AoiSystem::~AoiSystem()
{
}

AoiSystem::AoiSystem()
{
}


bool AoiSystem::EnterAoi(engine::ecs::GameObjectSPtr aoi_entity, engine::ecs::GameObjectSPtr npc, util::vector::Vector3 drop_point)
{
    bool success = false;
    do
    {
        auto aoi_comp = GetAoiObjectComponent(npc);
        if(aoi_comp == nullptr)
            break;

        auto aoi = GetAoiComponent(aoi_entity);
        if (aoi == nullptr)
            break;

        int id = aoi_comp->GetObjId();
        if(id < 0)
            break;

        engine::ecs::GameObjectSPtr old_obj = GetGameObj(aoi_entity, id);
        if(bbt_unlikely(old_obj == nullptr))
            LeaveAoi(aoi_entity, npc);
        
        /* 获取灯塔下标 */
        auto tower = GetTowerByPos3(drop_point);
        if(bbt_unlikely(tower == nullptr))
            break;

        /* 移动aoi obj */
        aoi_comp->OnMove(drop_point);
        aoi_comp->OnMove(tower);

        /* 将player加入aoi中管理 */
        auto isok = aoi->m_gameobj_map.Insert(id, npc);
        if(bbt_unlikely(!isok))
            break;

        /* 将player加入到灯塔中 */
        auto [_,isok2] = tower->m_players.insert(std::make_pair(id, npc));
        if(bbt_unlikely(!isok2))
            break;

        success = true;
        /* 通知九宫格所有人 */
        aoi->ScanTowerAround(tower, [this, npc, aoi_entity](Tower* tower, int n){
            EnterTowerBroadCast(aoi_entity, npc, tower, n);
        });

        OnEnter(npc);
    }while(0);
    
    return success;
}

bool AoiSystem::LeaveAoi(GameObjectSPtr aoi_entity, GameObjectSPtr player)
{
    bool success = false;
    do
    {
        auto aoi_comp = GetAoiObjectComponent(player);
        if(bbt_unlikely(aoi_comp == nullptr))
            break;

        auto aoi = GetAoiComponent(aoi_entity);
        if (aoi == nullptr)
            break;

        int id = aoi_comp->GetObjId();
        if(bbt_unlikely(id < 0))
            break;

        engine::ecs::GameObjectSPtr old_obj = GetGameObj(aoi_entity, id);
        if(bbt_unlikely(old_obj == nullptr))
            break;

        auto tower = GetTowerByPos3(aoi_comp->GetCurrentPos());
        if(bbt_unlikely(tower == nullptr))
            break;
        
        /* 将player从aoi中删除 */
        auto [_, isok] = aoi->m_gameobj_map.Earse(id);
        if(bbt_unlikely(!isok))
            break;
        
        /* 将player从灯塔中删除 */
        auto a = RemoveObjFromTowerById(tower, id);
        if(bbt_unlikely(a == nullptr))
            break;            

        success = true;
        /* 通知灯塔范围内的所有人 */
        aoi->ScanTowerAround(tower, [this, player, aoi_entity](Tower* tower, int n){
            LeaveTowerBroadCast(aoi_entity, player, tower, n);
        });

        OnLeave(player);
    }while(0);

    return success;
}

bool AoiSystem::Move(GameObjectSPtr aoi_entity, GameObjectSPtr player, util::vector::Vector3 moveto)
{
    bool success = false;
    do
    {
        /* 是否可以看做aoi中的对象 */
        auto aoi_comp = GetAoiObjectComponent(player);
        if (bbt_unlikely(aoi_comp == nullptr))
            break;

        auto aoi = GetAoiComponent(aoi_entity);
        if (bbt_unlikely(aoi == nullptr))
            break;

        /* id是否合法 */
        int id = aoi_comp->GetObjId();
        if(bbt_unlikely(id < 0))
            break;

        auto old_pos = aoi_comp->GetCurrentPos();        
        /* 是否为aoi中对象，debug检测 */
        DebugAssertWithInfo(GetGameObj(aoi_entity, id) != nullptr , "aoi object not found");

        /* 新灯塔插入，旧灯塔删除 */
        auto new_tower = GetTowerByPos3(moveto);
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
        aoi_comp->OnMove(moveto);
        aoi_comp->OnMove(new_tower);

        /* 通知灯塔范围内的玩家 */
        if(bbt_unlikely(new_tower == old_tower))
            break;

        success = true;
        /* 广播新灯塔九宫格 */
        aoi->ScanTowerAround(new_tower, [this, player, aoi_entity](Tower* tower, int n){
            this->EnterTowerBroadCast(aoi_entity, player, tower, n);
        });

        /* 广播旧灯塔九宫格 */
        aoi->ScanTowerAround(old_tower, [this, player, aoi_entity](Tower* tower, int n){
            this->LeaveTowerBroadCast(aoi_entity, player, tower, n);
        });
    }while(0);
    
    return success;
}


void AoiSystem::OnEnter(GameObjectSPtr player)
{
}

void AoiSystem::OnLeave(GameObjectSPtr player)
{
}


Tower* AoiSystem::GetTowerByPos3(util::vector::Vector3 pos3)
{

}

engine::ecs::GameObjectSPtr AoiSystem::RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id)
{
    if(from_tower == nullptr || from_tower->m_players.empty())
        return nullptr;
        
    auto it = from_tower->m_players.find(id);
    if(it == from_tower->m_players.end())
        return nullptr;
    
    from_tower->m_players.erase(it);    
    return it->second;
}

bool AoiSystem::InsertObj2Tower(Tower* to_tower, AoiObjectId key, engine::ecs::GameObjectSPtr obj)
{
    if(to_tower == nullptr || key < 0 || obj == nullptr)
        return false;

#ifdef Debug
    /* debug阶段做好，我们可以保证runtime不会出问题。这样就不需要多次取id */
    auto aoi_comp = GetAoiObjectComponent(obj);
    DebugAssertWithInfo(aoi_comp != nullptr, "gameobj has no AoiComponent!");
    DebugAssertWithInfo(aoi_comp->GetObjId() == key, "aoi id not equal!");
#endif

    auto it = to_tower->m_players.find(key);
    if(it != to_tower->m_players.end())
        return false;
    
    to_tower->m_players.insert(std::make_pair(key, obj));
    return true;
}


bool AoiSystem::HasAoiComponent(engine::ecs::GameObjectSPtr obj)
{
    if(obj == nullptr)
        return false;
    auto aoi_comp = obj->GetComponent(m_comp_template_id);
    if(aoi_comp == nullptr)
        return false;
    
    return true;
}

std::shared_ptr<ecs::aoi::AoiComponent> AoiSystem::GetAoiComponent(engine::ecs::GameObjectSPtr obj)
{
    if(obj == nullptr)
        return nullptr;

    auto comp = obj->GetComponent(m_comp_template_id);
    if(comp == nullptr)
        return nullptr;

    return std::static_pointer_cast<ecs::aoi::AoiComponent>(comp); 
}

std::shared_ptr<ecs::aoi::AoiObjectComponent> AoiSystem::GetAoiObjectComponent(engine::ecs::GameObjectSPtr obj)
{
    if (obj == nullptr) 
        return nullptr;
    
    auto component = obj->GetComponent(m_aoiobj_template_id);
    if (component == nullptr)
        return nullptr;
    
    return std::static_pointer_cast<ecs::aoi::AoiObjectComponent>(component);
}

engine::ecs::GameObjectSPtr AoiSystem::GetGameObj(engine::ecs::GameObjectCSPtr aoi, AoiObjectId id)
{
    auto aoi_component = GetAoiComponent(aoi);
    if (!aoi_component)
        return nullptr;

    auto [gameobj, isexist] = aoi_component->m_gameobj_map.Find(id);
    if (!isexist)
        return nullptr;

    return gameobj;
}

void AoiSystem::EnterTowerBroadCast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n)
{
    /**
     * 当一个实体进入灯塔范围的时候，应该通知之前的灯塔中所有的观察者。
     * 并通知实体自己，这样才能做到实体移动过程中可以被移动前后区域感知
     */
    auto aoi_obj_component = GetAoiObjectComponent(player);
    auto aoi_comp = GetAoiComponent(aoi);
    if(bbt_unlikely(aoi_obj_component == nullptr))
        return;

    for (auto it_tplayer : tower->m_players)
    {
        if(it_tplayer.first == aoi_obj_component->GetObjId())
            return;
            
        auto it_comp = GetAoiObjectComponent(it_tplayer.second);
        if(it_comp == nullptr)
            return;
            
        if( aoi_obj_component->GetMode() & AoiEntityFlag::Watcher )
            aoi_comp->m_enter_func(player, it_tplayer.second);
            
        if( it_comp->GetMode() & AoiEntityFlag::Watcher )
            aoi_comp->m_enter_func(it_tplayer.second, player);
    }
}

void AoiSystem::LeaveTowerBroadCast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n)
{

}

} // namespace share::ecs::aoi

