/**
 * @file CalcOpt.hpp
 * @author yqm-307 (you@domain.com)
 * @brief 坐标相关的计算
 * @version 0.1
 * @date 2023-12-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include "Pos3.hpp"
#include "Vector3.hpp"
#include <cmath>
#include <math.h>


//TODO 坐标计算库
namespace util::vector
{

/**
 * @brief 根据向量 vec 和原坐标点 pos，计算出 pos 在根据 vec 平移后的结果
 * 
 * @param pos 
 * @param vec 
 * @param len 
 * @return 
 */
inline static pos::Point3 CalcPos3ByVector(pos::Point3 pos, Vector3 vec)
{
    return {
        pos.m_x + vec.m_x,
        pos.m_y + vec.m_y,
        pos.m_z + vec.m_z
    };
}

/**
 * @brief 计算从 head 到 tail 的向量
 * 
 * @param head 初始点
 * @param tail 结束点
 * @return pos::Point3
 */
inline static vector::Vector3 CalcVectorByPos(pos::Point3 head, pos::Point3 tail)
{
    pos::Point3 pos = tail - head;

    return vector::Vector3{
        pos.m_x,
        pos.m_y,
        pos.m_z,
    };
}

/**
 * @brief 返回向量 vector 的模长
 */
inline static float CalcNormOfVector(vector::Vector3 vector)
{
    return std::abs(std::sqrt( 
        vector.m_x * vector.m_x +
        vector.m_y * vector.m_y + 
        vector.m_z * vector.m_z)); 
};

/**
 * @brief 根据 vector 和新的模长 norm 计算出新的 vector 并返回
 * 
 * @param vector 
 * @param norm 
 * @return vector::Vector3 
 */
inline static vector::Vector3 ChangeVectorNorm(vector::Vector3 vector, float norm)
{ 
    float rate = 0.0f;
    rate = CalcNormOfVector(vector) / norm;

    return {
        vector.m_x * rate,
        vector.m_y * rate,
        vector.m_z * rate
    };
}

/**
 * @brief 获取点 pos 在x、y、z轴上，按照根据dist_x、dist_y、dist_z平移后的坐标
 * 
 * @param pos 
 * @param dist_x 
 * @param dist_y 
 * @param dist_z 
 * @return pos::Index3 
 */
inline static pos::Point3 PointTranslate(pos::Point3 pos, float dist_x, float dist_y, float dist_z)
{ 
    return {  
        pos.m_x + dist_x, 
        pos.m_y + dist_y, 
        pos.m_z + dist_z}; 
}



} // namespace util::vector