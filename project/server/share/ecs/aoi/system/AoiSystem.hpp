#pragma once
#include <memory>
#include "share/ecs/aoi/component/AoiComponent.hpp"
#include "share/ecs/aoi/component/AoiObjectComponent.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::aoi
{

class AoiSystem
{
    typedef std::vector<engine::ecs::GameObjectSPtr>     EntityResult;
    typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
    typedef std::function<void(Tower*, int)>  AroundFunc;     /* 环视函数, 被扫到的灯塔，第几个（0-8） */
    /* 在aoi中，通知p2进入p1的关注范围 */
    typedef std::function<void(GameObjectSPtr/*aoi*/, GameObjectSPtr/*p1*/, GameObjectSPtr/*p2*/)> OnEnterFunc;
    /* 在aoi中，通知p2离开p1的关注范围 */
    typedef std::function<void(GameObjectSPtr/*aoi*/, GameObjectSPtr/*p1*/, GameObjectSPtr/*p2*/)> OnLeaveFunc;

public:
    ~AoiSystem();
    static std::unique_ptr<AoiSystem>& GetInstance();

    /* 实体进入aoi */
    bool EnterAoi(GameObjectSPtr aoi_entity, GameObjectSPtr entity, util::vector::Vector3 drop_point);
    /* 实体离开aoi */
    bool LeaveAoi(GameObjectSPtr aoi_entity, GameObjectSPtr player);
    /* 实体在aoi中移动 */
    bool Move(GameObjectSPtr aoi_entity, GameObjectSPtr player, util::vector::Vector3 moveto);

    GameObjectSPtr GetGameObjByAoi(engine::ecs::GameObjectCSPtr aoi, AoiObjectId id);

    void SetOnEnterAoiEvent(const OnEnterFunc& func);
    void SetOnLeaveAoiEvent(const OnLeaveFunc& func);
private:
    /* p2进入p1的感兴趣区域（aoi）时调用 */
    void OnEnterAoi(GameObjectSPtr aoi_entity, GameObjectSPtr p1, GameObjectSPtr p2);
    /* p2离开p1的感兴趣区域（aoi）时调用 */
    void OnLeaveAoi(GameObjectSPtr aoi_entity, GameObjectSPtr p1, GameObjectSPtr p2);
    void EnterTowerBroadcast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n);
    void LeaveTowerBroadcast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n);

    /* 根据id从灯塔中删除一个aoi对象，失败返回nullptr */
    GameObjectSPtr RemoveObjFromTowerById(Tower* from_tower, AoiObjectId id);
    bool InsertObj2Tower(Tower* to_tower, AoiObjectId key, GameObjectSPtr value);
    void OnEnter(GameObjectSPtr player);
    void OnLeave(GameObjectSPtr player);

private:
    bool HasAoiComponent(GameObjectSPtr aoi_entity);
    std::shared_ptr<ecs::aoi::AoiComponent> GetAoiComponent(GameObjectSPtr obj);
    std::shared_ptr<ecs::aoi::AoiObjectComponent> GetAoiObjectComponent(GameObjectSPtr obj);
protected:
    AoiSystem();


private:
    const engine::ecs::ComponentTemplateId m_comp_template_id{share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI};
    const engine::ecs::ComponentTemplateId m_aoiobj_template_id{share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT};

    OnLeaveFunc m_onleave_aoi_notify_event{nullptr};
    OnEnterFunc m_onenter_aoi_notify_event{nullptr};


};

} // namespace share::ecs::aoi

