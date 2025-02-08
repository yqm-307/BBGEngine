#pragma once
#include "engine/ecs/component/Component.hpp"
#include "plugin/ecs/attribute/AttrModule.hpp"

#define BBG_ATTRIBUTE_GSETTER(attr_name, type, attr_set_name, attr_set_field_name) \
    type Get##attr_name() { return attr_set_name.##attr_set_field_name; } \
    void Set##attr_name(type value) { attr_set_name.##attr_set_field_name = value; }

namespace share::ecs::attr
{
/**
 * @brief 属性
 * 
 * //TODO 提供各种属性变动的回调
 * 
 */
class Attribute:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    typedef std::function<void(AttributeType, int64_t)> OnAttributeChangeEvent;

    Attribute();
    ~Attribute();

    bool                RegistModule(AttributeModuleType type, AttrModule::SPtr module);
    void                OnRealAttributeChange(AttributeType type, int64_t change_value);
private:
    virtual void        OnUpdate() override;
    void                RecalcAllModuleAttr();
    /* 根据max属性矫正real属性，矫正属性是否属于属性修改呢？ */
    void                CorrentRealAttr(AttributeData* max_attr, AttributeData* real_attr);
private:
    std::unordered_map<AttributeModuleType, AttrModule::SPtr>   m_attr_module_map;
    AttributeData*                                              m_max_attribute{nullptr};           // 当前属性集
    AttributeData*                                              m_real_attribute{nullptr};          // 真实属性集
    std::unordered_map<AttributeType, OnAttributeChangeEvent>   m_attribute_listen_events_map;      // 属性监听事件集
};

} // namespace share::ecs::attribute

#ifdef BBG_ATTRIBUTE_GSETTER
#undef BBG_ATTRIBUTE_GSETTER
#endif