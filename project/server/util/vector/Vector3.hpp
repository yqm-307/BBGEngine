#pragma once
#include "util/Define.hpp"


namespace util::vector
{

struct Vector3
{
    Vector3 operator-(const Vector3& other);
    Vector3 operator+(const Vector3& other);
    /* 精度在 1e-5 */
    bool    operator==(const Vector3& other) const;
    /* 精度在 1e-5 */
    bool    operator!=(const Vector3& other) const;
    /* 逐比特比较，完全相等 */
    bool    Equals(const Vector3& other) const;
    /* 近似，有误差的比较x、y、z的值 */
    bool    Approx(const Vector3& other, float deviation_value = FLOAT_EQUAL_DIFF_VALUE) const;
    /* 非安全的，有溢出风险 */
    float   Norm() const;

    float m_x{-1.0f};
    float m_y{-1.0f};
    float m_z{-1.0f};

};

static inline bool Vec3IsNull(Vector3 v3)
{
    return v3.m_x == -1.0f && v3.m_y == -1.0f && v3.m_z == -1.0f;
}

}