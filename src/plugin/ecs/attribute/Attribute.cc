#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/attribute/Attribute.hpp"

namespace plugin::ecs::attr
{

Attribute::Attribute():
    engine::ecs::Component(EM_COMPONENT_TYPE_ATTRIBUTE)
{
}

Attribute::~Attribute()
{
}

void Attribute::OnUpdate()
{
    bool need_update = false;

    for (auto it : m_attr_module_map) {
        auto module = it.second; 
        if (module->IsModify()) {
            module->Recalculate();
            module->MarkModify(false);
        }
    }

    if (need_update) {
        RecalcAllModuleAttr();
    }
}

bool Attribute::RegistModule(AttributeModuleType type, AttrModule::SPtr module)
{
    auto [it, isok] = m_attr_module_map.insert(std::make_pair(type, module));
    return isok;
}

void Attribute::RecalcAllModuleAttr()
{
    static AttributeData total_attr;
    for (auto it : m_attr_module_map) {
        auto attr = it.second->GetAttributeData();
        total_attr += (*attr);
    }

    (*m_max_attribute) = total_attr;

    // 根据最新属性，矫正real属性
    CorrentRealAttr(m_max_attribute, m_real_attribute);
}

void Attribute::CorrentRealAttr(AttributeData* max_attr, AttributeData* real_attr)
{
    if (real_attr->m_base_hp >= max_attr->m_base_hp) {
        real_attr->m_base_hp = max_attr->m_base_hp;
    }

    if (real_attr->m_base_mp >= max_attr->m_base_mp) {
        real_attr->m_base_mp = max_attr->m_base_mp;
    }
}





}