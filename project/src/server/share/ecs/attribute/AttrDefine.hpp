#pragma once
#include <iostream>
#include <memory>

/**
 *  Attribute Module的设计
 * 
 *  将Attribute组件挂载到npc上，会赋予npc属性相关的功能
 * 
 *  我们在初始化的时候需要将每个属性模块注册到Attribute组件上
 */

namespace share::ecs::attr
{

enum AttributeType: int32_t
{
    BASE_HP             = 1,
    BASE_MP             = 2,
    BASE_ATTACK         = 3,
};

struct AttributeData
{
    int64_t             m_base_hp{0};       // 基础生命值           
    int64_t             m_base_mp{0};       // 基础法力值
    int64_t             m_base_attack{0};   // 基础攻击力

    void Clear()
    {
        m_base_hp = 0;
        m_base_mp = 0;
        m_base_attack = 0;
    }

    AttributeData operator+(const AttributeData& other)
    {
        AttributeData data;
        data += *this;
        data += other;

        return data;
    }

    AttributeData& operator+=(const AttributeData& other)
    {
        m_base_hp += other.m_base_hp;
        m_base_mp += other.m_base_mp;
        m_base_attack += other.m_base_attack;

        return *this;
    }

    AttributeData& operator=(const AttributeData& other)
    {
        this->m_base_hp = other.m_base_hp;
        this->m_base_mp = other.m_base_mp;
        this->m_base_attack = other.m_base_attack;
        return *this;
    }
};

enum AttributeModuleType: int
{
};

} // namespace share::ecs::attribute