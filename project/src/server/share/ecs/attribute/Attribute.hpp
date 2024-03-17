#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/attribute/AttrDefine.hpp"

#define BBG_ATTRIBUTE_GSETTER(attr_name, type, attr_set_name, attr_set_field_name) \
    type Get##attr_name() { return attr_set_name.##attr_set_field_name; }; \
    void Set##attr_name(type value) { attr_set_name.##attr_set_field_name = value; }

namespace share::ecs::npc
{

class Attribute:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    BBG_ATTRIBUTE_GSETTER( BaseAtk, int64_t, m_base_attr, m_base_attack );
    BBG_ATTRIBUTE_GSETTER( BaseHP, int64_t, m_base_attr, m_base_hp );
    BBG_ATTRIBUTE_GSETTER( BaseMP, int64_t, m_base_attr, m_base_mp );
private:
    virtual void OnUpdate() override {};
private:
    AttributeImpl m_base_attr;  // 基础属性
};

} // namespace share::ecs::attribute

#ifdef BBG_ATTRIBUTE_GSETTER
#undef BBG_ATTRIBUTE_GSETTER
#endif