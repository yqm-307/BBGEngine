#pragma once
#include <memory>
#include "plugin/ecs/aoi/AoiComponent.hpp"
#include "plugin/ecs/aoi/AoiObjectComponent.hpp"
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::aoi
{

class AoiSystem
{
    typedef std::vector<engine::ecs::EntitySPtr>     EntityResult;
    typedef engine::ecs::EntitySPtr EntitySPtr;
    typedef std::function<void(Tower*, int)>  AroundFunc;     /* 环视函数, 被扫到的灯塔，第几个（0-8） */
    /* 在aoi中，通知p2进入p1的关注范围 */
    typedef std::function<void(EntitySPtr/*aoi*/, EntitySPtr/*p1*/, EntitySPtr/*p2*/)> OnEnterFunc;
    /* 在aoi中，通知p2离开p1的关注范围 */
    typedef std::function<void(EntitySPtr/*aoi*/, EntitySPtr/*p1*/, EntitySPtr/*p2*/)> OnLeaveFunc;

public:
    ~AoiSystem();
    static std::unique_ptr<AoiSystem>& GetInstance();

    /* 实体进入aoi */
    bool EnterAoi(EntitySPtr aoi_entity, EntitySPtr entity, util::pos::Point3 drop_point);
    /* 实体离开aoi */
    bool LeaveAoi(EntitySPtr aoi_entity, EntitySPtr player);
    /* 实体在aoi中移动 */
    bool Move(EntitySPtr aoi_entity, EntitySPtr player, util::pos::Point3 moveto);

    EntitySPtr GetGameObjByAoi(engine::ecs::EntityCSPtr aoi, AoiObjectId id);

    void SetOnEnterAoiEvent(const OnEnterFunc& func);
    void SetOnLeaveAoiEvent(const OnLeaveFunc& func);
private:
    /* p2进入p1的感兴趣区域（aoi）时调用 */
    void OnEnterAoi(EntitySPtr aoi_entity, EntitySPtr p1, EntitySPtr p2);
    /* p2离开p1的感兴趣区域（aoi）时调用 */
    void OnLeaveAoi(EntitySPtr aoi_entity, EntitySPtr p1, EntitySPtr p2);
    void EnterTowerBroadcast(EntitySPtr aoi, EntitySPtr player, Tower* tower, int n);
    void LeaveTowerBroadcast(EntitySPtr aoi, EntitySPtr player, Tower* tower, int n);

    /* 根据id从灯塔中删除一个aoi对象，失败返回nullptr */
    EntitySPtr RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id);
    bool InsertObj2Tower(Tower* to_tower, AoiObjectId key, EntitySPtr value);
    void OnEnter(EntitySPtr player);
    void OnLeave(EntitySPtr player);

private:
    bool HasAoiComponent(EntitySPtr aoi_entity);
    std::shared_ptr<ecs::aoi::AoiComponent> GetAoiComponent(EntitySPtr obj);
    std::shared_ptr<ecs::aoi::AoiObjectComponent> GetAoiObjectComponent(EntitySPtr obj);
protected:
    AoiSystem();


private:
    const engine::ecs::ComponentTemplateId m_comp_template_id{plugin::ecs::emComponentType::EM_COMPONENT_TYPE_AOI};
    const engine::ecs::ComponentTemplateId m_aoiobj_template_id{plugin::ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT};

    OnLeaveFunc m_onleave_aoi_notify_event{nullptr};
    OnEnterFunc m_onenter_aoi_notify_event{nullptr};


};

} // namespace share::ecs::aoi

