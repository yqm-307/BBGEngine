#pragma once
#include "share/ecs/Component.hpp"
#include "share/ecs/system/aoi/Define.hpp"
#include "util/vector/Vector3.hpp"


namespace game::share::ecs::component
{

class AoiComponent:
    public game::share::ecs::Component
{
public:
    AoiComponent();
    virtual ~AoiComponent();
    virtual void OnAddComponent();
    virtual void OnDelComponent();
    virtual void OnUpdate();
    /* 获取组件的 aoi object id */
    int GetObjId() const;
    /* 获取实体的身份 */
    int GetEntityMode() const;
    /* 获取aoi当前位置 */
    util::vector::Vector3 GetCurrentPos() const;
private:
    int     m_aoi_object_id;    // aoi 对象id
    int     m_aoi_entity_mode;  // aoi 实体身份      
    /* 视野范围 */
    float           m_view_range_x;
    float           m_view_range_y;
    float           m_view_range_z;
    /* 实体大小 */
    system::AABBBox   m_aabb_box;
    util::vector::Vector3 m_prev_pos;  // 上一帧位置
    util::vector::Vector3 m_current_pos;   // 当前帧位置
    /* 位移请求队列: 缓存客户端发来的位移请求，并在World Update时执行 */
};

}