#pragma once

namespace util::pos
{

struct Index3
{


    int x{-1};
    int y{-1};
    int z{-1};
};


/**
 * @brief 三维空间坐标系中表示一个坐标点
 */
struct Point3
{
    Point3 operator-(const Point3& other);
    Point3 operator+(const Point3& other);

    float m_x{-1.0f};
    float m_y{-1.0f};
    float m_z{-1.0f};
};


}