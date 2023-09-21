#include "engine/ecs/component/Component.hpp"

using namespace engine::ecs;

Component::Component(std::string name, ComponentTemplateId id)
    :m_name(name),
    m_template_id(id)
{
    OnCreate();
}


Component::~Component()
{
    OnDestory();
}

const std::string& Component::GetName() const
{
    return m_name;
}

ComponentTemplateId Component::GetTemplateId() const
{
    return m_template_id;
}

void Component::SetId(ComponentId id)
{
    m_id = id;
}

