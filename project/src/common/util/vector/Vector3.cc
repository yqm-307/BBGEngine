#include "Vector3.hpp"
#include <cmath>
#include <string.h>

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

bool Vector3::operator==(const Vector3& other) const
{
    return std::abs(Norm() - other.Norm()) < FLOAT_EQUAL_DIFF_VALUE;
}

bool Vector3::operator!=(const Vector3& other) const
{
    return !(*this == other);
}

bool Vector3::Equals(const Vector3& other) const
{
    return (memcmp(&other, this, sizeof(Vector3)) == 0);
}

bool Vector3::Approx(const Vector3& other, float deviation_value) const
{
    return (
        std::abs(m_x - other.m_x) < deviation_value &&
        std::abs(m_y - other.m_y) < deviation_value &&
        std::abs(m_z - other.m_z) < deviation_value
    );
}


float Vector3::Norm() const
{
    return std::abs(std::sqrt( 
        m_x * m_x +
        m_y * m_y + 
        m_z * m_z));
}


}