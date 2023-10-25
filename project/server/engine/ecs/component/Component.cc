#include "engine/ecs/component/Component.hpp"

namespace engine::ecs
{


Component::Component(ComponentTemplateId id)
    :m_template_id(id)
{
    OnCreate();
}


Component::~Component()
{
    OnDestory();
}

void Component::OnCreate()
{
}

const std::string& Component::GetName() const
{
    return G_ComponentMgr()->GetComponentName(m_template_id);
}

ComponentTemplateId Component::GetTemplateId() const
{
    return m_template_id;
}

ComponentId Component::GetId() const
{
    return GetMemberId();
}

}



