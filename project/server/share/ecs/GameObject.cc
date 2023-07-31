#include "share/ecs/GameObject.hpp"
#include "util/assert/Assert.hpp"

using namespace game::share::ecs;


GameObject::GameObject(GameObjType gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type > 0, "game object type error");
}

GameObject::~GameObject()
{
}

Component::SPtr GameObject::GetComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
        
    return it->second;
}

Component::SPtr GameObject::DelComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
    
    m_component_map.erase(it);
    it->second->OnDelComponent(shared_from_this());
    return it->second;
}


bool GameObject::AddComponent(Component::SPtr component)
{
    if(component == nullptr)
        return false;

    ComponentTemplateId tid = component->GetTemplateId();
    auto it = m_component_map.insert(std::make_pair(tid, component));
    component->OnAddComponent(shared_from_this());
    return it.second;
}

GameObjType GameObject::Type()
{
    return m_gobj_type;
}
