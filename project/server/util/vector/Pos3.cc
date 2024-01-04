#include "Pos3.hpp"
#include <cmath>

namespace util::pos
{


Point3 Point3::operator-(const Point3& other)
{
    return {
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z,
    };
}

Point3 Point3::operator+(const Point3& other)
{
    return {
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z,
    };
}

bool Point3::operator==(const Point3& other) const
{
    return {
        std::abs(m_x - other.m_x) < FLOAT_EQUAL_DIFF_VALUE &&
        std::abs(m_y - other.m_y) < FLOAT_EQUAL_DIFF_VALUE &&
        std::abs(m_z - other.m_z) < FLOAT_EQUAL_DIFF_VALUE
    };
}

bool Point3::operator!=(const Point3& other) const
{
    return !(*this == other);
}


}