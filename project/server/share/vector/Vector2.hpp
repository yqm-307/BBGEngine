#pragma once

namespace game::share::vector
{

class Vector2
{
public:
    Vector2();
    Vector2(int x = 0, int y = 0);
    ~Vector2();

    inline int GetX(int x);
    inline int GetY(int y);
    inline void SetX(int x);
    inline void SetY(int y);
private:
    int m_x;
    int m_y;
};

}