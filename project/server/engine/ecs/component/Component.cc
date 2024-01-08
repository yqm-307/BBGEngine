#include "engine/ecs/component/Component.hpp"

namespace engine::ecs
{


Component::Component(ComponentTemplateId id)
    :m_template_id(id),
    m_is_active(true)
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

void Component::OnDestory()
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

void Component::OnAddComponent(ecs::GameObjectSPtr)
{
}

void Component::OnDelComponent(ecs::GameObjectSPtr)
{
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}

void Component::SetActive(bool is_active)
{
    if (m_is_active == is_active)
        return;

    if (is_active)
        OnEnable();
    else
        OnDisable();

    m_is_active = is_active;
}

bool Component::IsActive()
{
    return m_is_active;
}

void Component::Update()
{
    if (IsActive())
        OnUpdate();
}



}



