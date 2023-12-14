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

public:
    ~AoiSystem();
    static std::unique_ptr<AoiSystem> GetInstance();

    /* entity降落在实体 aoi_entity中的 drop_point处 */
    bool EnterAoi(GameObjectSPtr aoi_entity, GameObjectSPtr entity, util::vector::Vector3 drop_point);
    bool LeaveAoi(GameObjectSPtr aoi_entity, GameObjectSPtr player);

    bool Move(GameObjectSPtr aoi_entity, GameObjectSPtr player, util::vector::Vector3 moveto);

    // EntityResult GetEntitysByPos(util::vector::Vector3 pos);
    // EntityResult GetEntitysByGameobj(GameObjectSPtr gameobj);
    // EntityResult GetEntitysByAoiObjId(AoiObjectId aoiobj_id);
    // bool CheckEntityIsInAoi(AoiObjectId aoiobj_id);
    // GameObjectSPtr GetEntityByAoiObjectId(AoiObjectId aoiobj_id);

    GameObjectSPtr GetGameObj(engine::ecs::GameObjectCSPtr aoi, AoiObjectId id);

private:
    void EnterTowerBroadCast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n);
    void LeaveTowerBroadCast(GameObjectSPtr aoi, GameObjectSPtr player, Tower* tower, int n);

    Tower* GetTowerByIndex3(util::pos::Index3 index3);
    /* 根据 pos 获取灯塔，失败返回nullptr */
    Tower* GetTowerByPos3(util::vector::Vector3 pos3);
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
    const engine::ecs::ComponentTemplateId m_comp_template_id = share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI;
    const engine::ecs::ComponentTemplateId m_aoiobj_template_id = share::ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT;
    

};

} // namespace share::ecs::aoi

