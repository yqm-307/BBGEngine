#include "share/ecs/GameObject.hpp"

using namespace game::share::ecs;


Component::SPtr GameObject::GetComponent(std::string component_name)
{
    if(component_name.empty())
        return nullptr;
    auto it = m_component_map.find(component_name);
    if(it != m_component_map.end())
        return it->second;
    return nullptr;
}

Component::SPtr GameObject::DelComponent(std::string component_name)
{
    auto it = m_component_map.find(component_name);
    if(it == m_component_map.end())
    {
        return nullptr;
    }
    m_component_map.erase(it);
    return it->second;
}


bool GameObject::AddComponent(Component::SPtr component)
{
    auto& name = component->GetName();
    auto it = m_component_map.insert(std::make_pair(name, component));
    return it.second;
}