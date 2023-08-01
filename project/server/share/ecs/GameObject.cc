#include "share/ecs/GameObject.hpp"
#include "util/assert/Assert.hpp"

namespace game::share::ecs
{



GameObject::GameObject(GameObjType gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type > 0, "game object type error");
}

GameObject::~GameObject()
{
}

ComponentSPtr GameObject::GetComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
        
    return it->second;
}

ComponentSPtr GameObject::DelComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
    
    m_component_map.erase(it);
    it->second->OnDelComponent(this);
    return it->second;
}


bool GameObject::AddComponent(ComponentSPtr component)
{
    if(component == nullptr)
        return false;

    ComponentTemplateId tid = component->GetTemplateId();
    auto it = m_component_map.insert(std::make_pair(tid, component));
    component->OnAddComponent(this);
    return it.second;
}

GameObjType GameObject::Type()
{
    return m_gobj_type;
}

}