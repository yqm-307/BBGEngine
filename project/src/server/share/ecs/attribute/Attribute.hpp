#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/attribute/AttrModule.hpp"

#define BBG_ATTRIBUTE_GSETTER(attr_name, type, attr_set_name, attr_set_field_name) \
    type Get##attr_name() { return attr_set_name.##attr_set_field_name; } \
    void Set##attr_name(type value) { attr_set_name.##attr_set_field_name = value; }

namespace share::ecs::attr
{

class Attribute:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;

    Attribute();
    ~Attribute();

    bool RegistModule(AttributeModuleType type, AttrModule::SPtr module);

private:
    virtual void OnUpdate() override;
    void RecalcAllModuleAttr();
private:
    std::unordered_map<AttributeModuleType, AttrModule::SPtr> m_attr_module_map;
    AttributeData m_current_attribute;
};

} // namespace share::ecs::attribute

#ifdef BBG_ATTRIBUTE_GSETTER
#undef BBG_ATTRIBUTE_GSETTER
#endif