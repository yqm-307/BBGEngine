#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/aoi/entity/Define.hpp"
#include "util/vector/Vector3.hpp"
#include "util/vector/Pos3.hpp"
#include "util/config/Config.hpp"


namespace share::ecs::aoi
{

class AoiSystem;
/**
 * 地图的aoi组件，保存地图中的玩家并维持兴趣列表
 */
class AoiComponent:
    public engine::ecs::Component
{
    friend class AoiSystem;
    ComponentDeriveClassDef;
    template<typename T> using Timestamp = bbt::timer::clock::Timestamp<T>;
    typedef util::hashmap::Hashmap<AoiObjectId, engine::ecs::GameObjectSPtr, AoiHashBucketNum> GameObjHashmap;    /* 游戏对象hash桶 */
    /* 为什么加扫描到的下标这个参数。因为后续可能做优化，现在可以预知扫描周围的人然后处理，会导致某个方向上的玩家收到信息较慢 */
    typedef std::function<void(Tower*, int)>  AroundFunc;     /* 环视函数, 被扫到的灯塔，第几个（0-8） */
    typedef std::function<void(engine::ecs::GameObjectSPtr/*p1*/, engine::ecs::GameObjectSPtr/*p2*/)>   OnEnterFunc;    /* 通知p1，p2进入了他的九宫格视野 */
    typedef std::function<void(engine::ecs::GameObjectSPtr/*p1*/, engine::ecs::GameObjectSPtr/*p2*/)>   OnLeaveFunc;    /* 通知p1，p2离开了他的九宫格视野 */
public:
    virtual ~AoiComponent();
    bool CheckConfig(const util::config::AoiConfig*) const;
    virtual void OnAddComponent(engine::ecs::GameObjectSPtr);
    virtual void OnDelComponent(engine::ecs::GameObjectSPtr);
    virtual void OnUpdate();

private:
    Tower* GetTowerByPos3(util::vector::Vector3 pos3);
    Tower* GetTowerByIndex3(util::pos::Index3 index3);
    util::pos::Index3   GetIndex3ByPos3(util::vector::Vector3 pos3) const;
    util::pos::Index3   GetIndex3ByIndex(int tower_index) const;
    engine::ecs::GameObjectSPtr GetGameObj(AoiObjectId id);
    void                ScanTowerAround(Tower* center_tower, AroundFunc dofunc);
    std::vector<engine::ecs::GameObjectSPtr>  GetEntitysEx(util::vector::Vector3 pos);
    bool                Move(engine::ecs::GameObjectSPtr player, util::vector::Vector3 moveto);
    void Init();

protected:
    explicit AoiComponent(OnEnterFunc onenter, OnLeaveFunc onleave);
private:
    /////////////////////////////////////////////////////////////////////////////
    int         m_tower_max_x;  // x 轴上灯塔数量
    int         m_tower_max_y;  // y 轴上灯塔数量
    int         m_tower_max_z;  // z 轴上灯塔数量
    static const engine::ecs::ComponentTemplateId    m_comp_template_id;    // 组件模板id
    Timestamp<bbt::timer::clock::ms>m_create_ms;    // Aoi创建时间 
    Timestamp<bbt::timer::clock::ms>m_prev_info_ms; // aoi定时输出当前信息，这个表示上次输出时间
    const std::string               m_comp_name;    // 组件名
    OnEnterFunc                     m_enter_func;
    OnLeaveFunc                     m_leave_func;
    GameObjHashmap                  m_gameobj_map; // AOI中所有游戏对象 hashmap
    std::vector<Tower>              m_towers;   // AOI中所有灯塔
    std::vector<MapSlot>            m_slots;    // AOI中所有Slot
    const util::config::AoiConfig*  m_config;   // AOI配置
};

}