#include "Pos3.hpp"

namespace util::pos
{


Point3 Point3::operator-(Point3 other)
{
    return {
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z,
    };
}

Point3 Point3::operator+(Point3 other)
{
    return {
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z,
    };
}

}