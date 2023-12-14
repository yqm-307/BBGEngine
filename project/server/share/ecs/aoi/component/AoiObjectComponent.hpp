#pragma once
#include "share/ecs/Define.hpp"
#include "share/ecs/aoi/entity/Define.hpp"

namespace share::ecs::aoi
{

/**
 * 在aoi中时保存位置信息
 */
class AoiObjectComponent
{
public:
    ~AoiObjectComponent();
    AoiObjectComponent();

    ecs::aoi::AoiEntityFlag GetMode();
    AoiObjectId GetObjId();
    
    void OnMove(util::vector::Vector3 vec);
    void OnMove(Tower* tower);
    util::vector::Vector3 GetCurrentPos();
private:

private:
    util::vector::Vector3 m_prev_pos;
    util::vector::Vector3 m_curr_pos;
    AoiObjectId m_id;   // aoi 对象id
    ecs::aoi::AoiEntityFlag m_mode; // 实体模式
};

} // namespace share::ecs::aoi