#pragma once
#include <iostream>

namespace share::ecs::npc
{

struct AttributeImpl
{
    int64_t             m_base_hp{0};       // 基础生命值           
    int64_t             m_base_mp{0};       // 基础法力值
    int64_t             m_base_attack{0};   // 基础攻击力
};

} // namespace share::ecs::attribute