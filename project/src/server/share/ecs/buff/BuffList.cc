#include "share/ecs/buff/BuffList.hpp"

namespace share::ecs::buff
{

void BuffList::OnUpdate()
{
    for (auto buff : m_buff_array) {
        buff->OnUpdate();
    }
}


}