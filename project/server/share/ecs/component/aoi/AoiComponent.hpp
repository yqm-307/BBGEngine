#pragma once
#include "share/ecs/Component.hpp"
#include "share/vector/Vector3.hpp"
#include "share/aoi/Define.hpp"

namespace game::share::ecs::component
{

class AoiComponent:
    public Component
{
public:
    AoiComponent();
    virtual ~AoiComponent();
    virtual void OnAddComponent();
    virtual void OnDelComponent();
    virtual void OnUpdate();
private:
    /* 当前位置坐标 */
    vector::Vector3 m_location;
    /* 上一帧位置坐标 */
    vector::Vector3 m_prev_location;
    /* 视野范围 */
    float           m_view_range_x;
    float           m_view_range_y;
    float           m_view_range_z;
    /* 实体大小 */
    aoi::AABBBox    m_aabb_box;

    /* 位移请求队列: 缓存客户端发来的位移请求，并在World Update时执行 */

};

}