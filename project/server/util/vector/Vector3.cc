#include "Vector3.hpp"

namespace util::vector
{

Vector3 Vector3::operator+(const Vector3& other)
{
    return {
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z
    };
}

Vector3 Vector3::operator-(const Vector3& other)
{
    return {
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z
    }; 
}
}