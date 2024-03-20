#include "share/ecs/Define.hpp"
#include "share/ecs/attribute/Attribute.hpp"

namespace share::ecs::attr
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



}