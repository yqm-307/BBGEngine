#pragma once
#include <map>
#include "share/vector/Vector2.hpp"
#include "share/aoi/Define.hpp"

namespace game::share::aoi
{

class Aoi
{
public:
    Aoi(int size[3], int tower[3]);
    ~Aoi();
private:
    size_t      m_length;
    MapSlot*    m_slots;
};

}