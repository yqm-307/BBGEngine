#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
namespace engine::ecs
{



GameObject::GameObject(int gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type > 0, "game object type error");
    OnBaseCreate();
}

GameObject::~GameObject()
{
    OnBaseDestory();
}

void GameObject::OnBaseCreate()
{
}

void GameObject::OnBaseDestory()
{
    GameObjectMgr::GetInstance()->OnDestoryGameObject(m_id);
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
    it->second->OnDelComponent(shared_from_this());
    return it->second;
}


bool GameObject::AddComponent(ComponentSPtr component)
{
    if(component == nullptr)
        return false;

    ComponentTemplateId tid = component->GetTemplateId();
    auto it = m_component_map.insert(std::make_pair(tid, component));
    component->OnAddComponent(shared_from_this());
    return it.second;
}

int GameObject::Type()
{
    return m_gobj_type;
}

GameObjectId GameObject::GetId()
{
    return m_id;
}

GameObjectSPtr GameObject::GetGameObject(GameObjectId id)
{
    if(m_childs.size() == 0 || GameObjectIDInvalid(id))
        return nullptr;

    auto it = m_childs.find(id);
    if(it == m_childs.end())
        return nullptr;

    DebugAssertWithInfo(!it->second->GetName().empty(), "obj is empty! maybe warning!");
#ifndef Debug
    if(it->second->GetName().empty())
        GAME_EXT1_LOG_WARN("Gameobject name is empty! child type: %d , parent type: ", it->second->Type(), m_gobj_type);
#endif

    return it->second;
}

const std::string& GameObject::GetName() const
{
    return m_gameobj_name;
}

bool GameObject::MountChild(GameObjectSPtr gameobj)
{
    auto objid = gameobj->GetId();
    if(GameObjectIDInvalid(objid))
        return false;

    auto [_, isok] = m_childs.insert(std::make_pair(objid, gameobj));
    
    return isok;
}

#pragma region "私有函数"

bool GameObject::HasGameobj(GameObjectId id) const
{
    if(GameObjectIDInvalid(id))
        return false;

    auto it = m_childs.find(id);
    if(it == m_childs.end())
        return false;

    return true;
}

void GameObject::SetId(GameObjectId id)
{
    m_id = id;
}

void GameObject::Update()
{
    for(auto child_ptr : m_childs)
    {
        auto gobj_ptr = child_ptr.second;

        /* 递归更新子组件 */
        gobj_ptr->Update();

    }

    /* 通知此组件已经更新了 */
    OnUpdate();
}


#pragma endregion

}