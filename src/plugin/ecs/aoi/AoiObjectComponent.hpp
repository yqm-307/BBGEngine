#pragma once
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/aoi/AoiDefine.hpp"
#include "util/vector/Vector3.hpp"
#include "util/vector/Pos3.hpp"
#include <stack>

namespace share::ecs::aoi
{

/**
 * 赋予父对象场景，内位置信息的能力
 */
class AoiObjectComponent: 
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    ~AoiObjectComponent();

    ecs::aoi::AoiEntityFlag GetMode();
    AoiObjectId GetObjId();
    engine::ecs::GameObjectSPtr GetCurrentAoi();
    
    void OnMove(util::pos::Point3 vec);
    void OnMove(Tower* tower);
    void OnEnterAoi(engine::ecs::GameObjectId aoi_gameobject_id);
    void OnLeaveAoi(engine::ecs::GameObjectId aoi_gameobject_id);
    util::pos::Point3 GetCurrentPos();
    
private:
    AoiObjectComponent(ecs::aoi::AoiEntityFlag mode);
    virtual void OnUpdate() override {}
    virtual void OnFatherDead() {};
    virtual void OnAddComponent(engine::ecs::GameObjectSPtr) {};
    virtual void OnDelComponent(engine::ecs::GameObjectSPtr) {};

private:
    engine::ecs::GameObjectId m_prev_aoi_gameobj_id{engine::ecs::InvalidGameObjectId};
    engine::ecs::GameObjectId m_curr_aoi_gameobj_id{engine::ecs::InvalidGameObjectId};
    Tower* m_curr_tower;
    util::pos::Point3 m_prev_pos;
    util::pos::Point3 m_curr_pos;
    AoiObjectId m_id;   // aoi 对象id
    ecs::aoi::AoiEntityFlag m_mode; // 实体模式
};

} // namespace share::ecs::aoi