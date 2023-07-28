#include "share/ecs/Component.hpp"

using namespace game::share::ecs;

Component::Component(std::string name)
    :m_name(name)
{
}


Component::~Component()
{
}

const std::string& Component::GetName() const
{
    return m_name;
}

ComponentTemplateId Component::GetTemplateId() const
{
    return m_template_id;
}
