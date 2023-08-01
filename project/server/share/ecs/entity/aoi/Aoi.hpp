#pragma once
#include <map>
#include <vector>
#include "share/ecs/entity/aoi/Define.hpp"
#include "util/config/config.hpp"
#include "util/hashmap/Hashmap.hpp"
#include "util/vector/Pos3.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::entity::aoi
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
    public GameObject
{
    typedef util::hashmap::Hashmap<AoiObjectId, ecs::GameObjectSPtr, AoiHashBucketNum> GameObjHashmap;    /* 游戏对象hash桶 */
    /* 为什么加扫描到的下标这个参数。因为后续可能做优化，现在可以预知扫描周围的人然后处理，会导致某个方向上的玩家收到信息较慢 */
    typedef std::function<void(Tower*, int)>  AroundFunc;     /* 环视函数, 被扫到的灯塔，第几个（0-8） */
    typedef std::function<void(ecs::GameObjectSPtr, ecs::GameObjectSPtr)>   OnEnterFunc;
    typedef std::function<void(ecs::GameObjectSPtr, ecs::GameObjectSPtr)>   OnLeaveFunc;
public:
    typedef std::vector<ecs::GameObjectSPtr>     EntityResult;

    static std::shared_ptr<Aoi> Create(OnEnterFunc onenter, OnLeaveFunc onleave);
    
    Aoi(OnEnterFunc onenter, OnLeaveFunc onleave);
    
    ~Aoi();
    /**
     * @brief 将玩家player放置到aoi中的drop_point位置，如果已经存在立即返回
     * 
     * @param player 玩家
     * @param drop_point 降落点
     */
    bool EnterAoi(ecs::GameObjectSPtr player, util::vector::Vector3 drop_point);
    
    /* 将player踢出aoi */
    bool LeaveAoi(ecs::GameObjectSPtr player);
    
    bool Move(ecs::GameObjectSPtr player, util::vector::Vector3 moveto);

    /**
     * @brief 获取坐标附近的九宫格的实体
     * 
     * @param pos 坐标位置
     * @return EntityResult  成功返回实体列表和true，否则返回空表和false。下同
     */
    EntityResult GetEntitysByPos(util::vector::Vector3 pos);

    /**
     * @brief 通过 gameobject 获取
     * 
     * @param gameobj 
     * @return EntityResult 
     */
    EntityResult GetEntitysByGameobj(ecs::GameObjectSPtr gameobj);
    
    /**
     * @brief 通过 aoi obj id 获取
     * 
     * @param aoiobj_id 
     * @return EntityResult 
     */
    EntityResult GetEntitysByAoiObjId(AoiObjectId aoiobj_id);

    /**
     * @brief 根据aoi obj id 判断是否存在于该aoi中
     * 
     * @param aoiobj_id 
     * @return true 
     * @return false 
     */
    bool CheckEntityIsInAoi(AoiObjectId aoiobj_id);

    ecs::GameObjectSPtr GetEntityByAoiObjectId(AoiObjectId aoiobj_id);
private:

    void Init();
    
    /* 进入地图 */
    void OnEnter(ecs::GameObjectSPtr player);
    
    void OnLeave(ecs::GameObjectSPtr player);
    
    void OnMove(ecs::GameObjectSPtr player);
    
    virtual void OnUpdate() override;
private:
    bool CheckConfig(const util::config::AoiConfig*) const;
private:
    /**
     * AOI 相关
     */

    /* 根据 pos 获取 index，失败返回{-1,-1,-1} */
    util::pos::Index3   GetIndex3ByPos3(util::vector::Vector3 pos3) const;
    
    /* 根据 tower 的 index，获取失败返回{-1,-1,-1} */
    util::pos::Index3   GetIndex3ByIndex(int tower_index) const;
    
    /* 根据 aoi object id 获取在aoi中的 gameobject对象。失败返回nullptr */
    ecs::GameObjectSPtr GetGameObj(AoiObjectId id);
    
    /* 环视 center_tower 灯塔周边的9宫格，并执行dofunc操作。 dofunc(Tower*, int)->void  */
    void                ScanTowerAround(Tower* center_tower, AroundFunc dofunc);
    
    /* player进入灯塔 tower 的关注范围，n是第几个灯塔 */
    void                EnterTowerBroadCast(ecs::GameObjectSPtr player, Tower* tower, int n);
    
    /* player离开灯塔 tower 的关注范围，n是第几个灯塔 */
    void                LeaveTowerBroadCast(ecs::GameObjectSPtr player, Tower* tower, int n);
    
    std::vector<ecs::GameObjectSPtr>  GetEntitysEx(util::vector::Vector3 pos);
private:
    /**
     *  Tower相关 
     */
    /* 根据 index 获取灯塔，失败返回nullptr */
    Tower*              GetTowerByIndex3(util::pos::Index3 index3);

    /* 根据 pos 获取灯塔，失败返回nullptr */
    Tower*              GetTowerByPos3(util::vector::Vector3 pos3);
    /* 根据id从灯塔中删除一个aoi对象，失败返回nullptr */
    ecs::GameObjectSPtr RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id);

    /* 向灯塔的实体列表中添加 aoi object id 和 实体 的键值对，失败返回false */
    bool                InsertObj2Tower(Tower* to_tower, AoiObjectId key, ecs::GameObjectSPtr value);

    /* Tower中的成员变动后修改 */
public:
    /* gameobj是否有AoiComponent组件 */
    static bool                HasAoiComponent(ecs::GameObjectSPtr obj);

    /* 从gameobj上获取AoiComponent组件，获取失败返回nullptr */
    static std::shared_ptr<ecs::component::AoiComponent> GetAoiComponent(ecs::GameObjectSPtr obj);
    
private:
    int         m_tower_max_x;  // x 轴上灯塔数量
    int         m_tower_max_y;  // y 轴上灯塔数量
    int         m_tower_max_z;  // z 轴上灯塔数量
    const std::string m_comp_name;    // 组件名
    static const ecs::ComponentTemplateId    m_comp_template_id;  // 组件模板id
    GameObjHashmap                  m_gameobj_map;          // AOI中所有游戏对象 hashmap
    std::vector<Tower>          m_towers;   // AOI中所有灯塔
    std::vector<MapSlot>        m_slots;    // AOI中所有Slot
    OnEnterFunc                 m_enter_func;
    OnLeaveFunc                 m_leave_func;
    const util::config::AoiConfig*      m_config;       // AOI配置
};

}// namespace game::share