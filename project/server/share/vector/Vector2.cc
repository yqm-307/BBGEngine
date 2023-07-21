#include "Vector2.hpp"

using namespace game::share::vector;

Vector2::Vector2()
    :m_x(0),
    m_y(0)
{}

Vector2::Vector2(int x, int y)
    :m_x(x),
    m_y(y)
{}

Vector2::~Vector2()
{
    m_x = 0;
    m_y = 0;
}

int Vector2::GetX(int x)
{
    return m_x;
}

int Vector2::GetY(int y)
{
    return m_y;
}

void Vector2::SetX(int x)
{
    m_x = x;
}

void Vector2::SetY(int y)
{
    m_y = y;
}