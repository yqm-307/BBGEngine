#pragma once
#include "share/ecs/Component.hpp"
#include "share/vector/Vector3.hpp"

namespace game::share::ecs::component
{

class AoiComponent:
    public Component
{
public:

private:
    /* 当前位置坐标 */
    vector::Vector3 m_location;
    /* 上一帧位置坐标 */
    vector::Vector3 m_prev_location;
    /* 视野范围 */
    float   m_view_range_x;
    float   m_view_range_y;
    float   m_view_range_z;
    
};

}