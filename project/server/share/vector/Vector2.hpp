#pragma once
#include "share/Define.hpp"

namespace game::share::vector
{

class Vector2
{
public:
    Vector2();
    Vector2(float x = 0, float y = 0);
    ~Vector2();

    inline float GetX(float x);
    inline float GetY(float y);
    inline void SetX(float x);
    inline void SetY(float y);    
private:
    float m_x;
    float m_y;
};

void MoveToPoint()
{
    
}

}