#include "Vector2.hpp"

using namespace game::share::vector;

Vector2::Vector2()
    :m_x(0),
    m_y(0)
{}

Vector2::Vector2(float x, float y)
    :m_x(x),
    m_y(y)
{}

Vector2::~Vector2()
{
    m_x = 0;
    m_y = 0;
}

float Vector2::GetX(float x)
{
    return m_x;
}

float Vector2::GetY(float y)
{
    return m_y;
}

void Vector2::SetX(float x)
{
    m_x = x;
}

void Vector2::SetY(float y)
{
    m_y = y;
}