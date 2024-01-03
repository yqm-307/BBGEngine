#pragma once

namespace util::vector
{

struct Vector3
{
    Vector3 operator-(const Vector3& other);
    Vector3 operator+(const Vector3& other);

    float m_x{-1.0f};
    float m_y{-1.0f};
    float m_z{-1.0f};

};

static inline bool Vec3IsNull(Vector3 v3)
{
    return v3.m_x == -1.0f && v3.m_y == -1.0f && v3.m_z == -1.0f;
}

}