#pragma once
#include <map>
#include <vector>
#include "./Define.hpp"
#include "util/config/config.hpp"
#include "util/hashmap/Hashmap.hpp"
#include "util/vector/Pos3.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::system
{

static const int AoiHashBucketNum = 16;

/**
 * @brief aoi中实体对象的身份标识
 */
enum AoiEntityFlag 
{
    Watcher     = 1,    // 观察者
    Marker      = 1 << 1,     // 被观察者
};


/**
 * @brief Aoi是基于笛卡尔坐标系. 九宫格实现的
 * 
 */
class Aoi:
    public bbt::templateutil::BaseType<Aoi>
{
    // friend class ecs::component::AoiComponent;
    typedef util::hashmap::Hashmap<AoiObjectId, ecs::GameObject::SPtr, AoiHashBucketNum> GameObjMap;    /* 游戏对象hash桶 */
    /* 为什么加扫描到的下标这个参数。因为后续可能做优化，现在可以预知扫描周围的人然后处理，会导致某个方向上的玩家收到信息较慢 */
    typedef std::function<void(Tower*, int)>  AroundFunc;     /* 环视函数, 被扫到的灯塔，第几个（0-8） */
    typedef std::function<void(ecs::GameObject::SPtr, ecs::GameObject::SPtr)>   OnEnterFunc;
    typedef std::function<void(ecs::GameObject::SPtr, ecs::GameObject::SPtr)>   OnLeaveFunc;
public:
    // static RawPtr GetInstance();
    static SPtr Create(OnEnterFunc onenter, OnLeaveFunc onleave);
    Aoi(OnEnterFunc onenter, OnLeaveFunc onleave);
    ~Aoi();
private:

    void Init();
    /**
     * @brief 将 player 放进aoi中的 drop_point 位置
     * 
     * @param player 游戏对象
     * @param drop_point 降落点
     */
    void EnterAoi(ecs::GameObject::SPtr player, util::vector::Vector3 drop_point);
    /* 将player踢出aoi */
    void LeaveAoi(ecs::GameObject::SPtr player);
    /* 进入地图 */
    void OnEnter(ecs::GameObject::SPtr player);
    void OnLeave(ecs::GameObject::SPtr player);
    void OnMove(ecs::GameObject::SPtr player);
    void OnUpdate(ecs::GameObject::SPtr player);
private:
    bool CheckConfig(const util::config::AoiConfig*);
private:
    /**
     * AOI 相关
     */
    /* 根据 index 获取灯塔，失败返回nullptr */
    Tower*              GetTowerByIndex3(util::pos::Index3 index3);
    /* 根据 pos 获取灯塔，失败返回nullptr */
    Tower*              GetTowerByPos3(util::vector::Vector3 pos3);
    /* 根据 pos 获取 index，失败返回{-1,-1,-1} */
    util::pos::Index3   GetIndex3ByPos3(util::vector::Vector3 pos3);
    /* 根据 tower 的 index，获取失败返回{-1,-1,-1} */
    util::pos::Index3   GetIndex3ByIndex(int tower_index);
    /* 根据 aoi object id 获取在aoi中的 gameobject对象。失败返回nullptr */
    ecs::GameObject::SPtr GetGameObj(AoiObjectId id);
    /* 环视 center_tower 灯塔周边的9宫格，并执行dofunc操作。 dofunc(Tower*, int)->void  */
    void                ScanTowerAround(Tower* center_tower, AroundFunc dofunc);
    /* player进入灯塔 tower 的关注范围，n是第几个灯塔 */
    void                EnterTower(ecs::GameObject::SPtr player, Tower* tower, int n);
private:
    bool                HasAoiComponent(ecs::GameObject::SPtr obj);
    std::shared_ptr<ecs::component::AoiComponent> GetAoiComponent(ecs::GameObject::SPtr obj);
private:
    size_t      m_length;
    int         m_tower_max_x;  // x 轴上灯塔数量
    int         m_tower_max_y;  // y 轴上灯塔数量
    int         m_tower_max_z;  // z 轴上灯塔数量
    const std::string m_comp_name;    // 组件名
    ecs::ComponentTemplateId    m_comp_template_id{ecs::ComponentTemplateId::EM_AoiComponent};  // 组件模板id
    GameObjMap                  m_gameobj_map;          // AOI中所有游戏对象 hashmap
    std::vector<Tower>          m_towers;   // AOI中所有灯塔
    std::vector<MapSlot>        m_slots;    // AOI中所有Slot
    OnEnterFunc                 m_enter_func;
    OnLeaveFunc                 m_leave_func;
    const util::config::AoiConfig*      m_config;       // AOI配置
    
};

}// namespace game::share