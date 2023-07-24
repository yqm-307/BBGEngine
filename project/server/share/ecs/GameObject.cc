#include "share/ecs/GameObject.hpp"
#include "util/assert/Assert.hpp"

using namespace game::share::ecs;


GameObject::GameObject(GameObjType gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type > 0, "game object type error");
}


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

GameObjType GameObject::Type()
{
    return m_gobj_type;
}
