#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/entity/aoi/Define.hpp"
#include "util/vector/Vector3.hpp"
#include "util/vector/Pos3.hpp"


namespace game::share::ecs::component
{

class AoiComponent:
    public game::share::ecs::Component
{
public:
    explicit AoiComponent();
    virtual ~AoiComponent();
    virtual void OnAddComponent(ecs::GameObjectSPtr);
    virtual void OnDelComponent(ecs::GameObjectSPtr);
    virtual void OnUpdate();
    /* 获取组件的 aoi object id */
    int GetObjId() const;
    /* 设置组件的 aoi object id */
    void SetObjId(int);

    /* 获取实体的身份 */
    int GetEntityMode() const;
    /* 获取aoi当前位置 */
    util::vector::Vector3 GetCurrentPos() const;
    /* 获取所在灯塔 */
    entity::aoi::Tower* GetTower() const;
    /* 移动至新位置 */
    void Moveto(util::vector::Vector3 new_pos);
    /* 移动至新的灯塔 */
    void Moveto(entity::aoi::Tower* new_tower);
    void Clean();

public:
    void Debug_PosChange();
private:
    int     m_aoi_object_id;    // aoi 对象id
    int     m_aoi_entity_mode;  // aoi 实体身份      
    entity::aoi::Tower*  m_tower;    // 所处的灯塔
    /* 实体大小 */
    entity::aoi::AABBBox  m_aabb_box;
    util::vector::Vector3 m_prev_pos;  // 上一帧位置
    util::vector::Vector3 m_current_pos;   // 当前帧位置
    /* 位移请求队列: 缓存客户端发来的位移请求，并在World Update时执行 */
};

}