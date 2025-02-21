#pragma once
#include <engine/ecs/component/Component.hpp>
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/aoi/AoiDefine.hpp"
#include "util/vector/Vector3.hpp"
#include "util/vector/Pos3.hpp"
#include <stack>

namespace plugin::ecs::aoi
{

/**
 * 赋予父对象场景，内位置信息的能力
 */
class AoiObjectComponent: 
    public engine::ecs::Component
{
    ComponentClassMetaInfo(EM_COMPONENT_TYPE_AOI_OBJECT);
public:
    ~AoiObjectComponent();

    ecs::aoi::AoiEntityFlag GetMode();
    AoiObjectId GetObjId();
    engine::ecs::EntitySPtr GetCurrentAoi();
    
    void OnMove(util::pos::Point3 vec);
    void OnMove(Tower* tower);
    void OnEnterAoi(engine::ecs::EntityId aoi_gameobject_id);
    void OnLeaveAoi(engine::ecs::EntityId aoi_gameobject_id);
    util::pos::Point3 GetCurrentPos();
    
private:
    AoiObjectComponent(ecs::aoi::AoiEntityFlag mode);
    virtual void OnUpdate() override {}
    virtual void OnFatherDead() {};
    virtual void OnAddComponent(engine::ecs::EntitySPtr) {};
    virtual void OnDelComponent(engine::ecs::EntitySPtr) {};

private:
    engine::ecs::EntityId m_prev_aoi_gameobj_id{engine::ecs::InvalidGameObjectId};
    engine::ecs::EntityId m_curr_aoi_gameobj_id{engine::ecs::InvalidGameObjectId};
    Tower* m_curr_tower;
    util::pos::Point3 m_prev_pos;
    util::pos::Point3 m_curr_pos;
    AoiObjectId m_id;   // aoi 对象id
    ecs::aoi::AoiEntityFlag m_mode; // 实体模式
};

} // namespace share::ecs::aoi