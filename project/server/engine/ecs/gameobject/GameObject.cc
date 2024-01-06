#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
namespace engine::ecs
{



GameObject::GameObject(int gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type >= 0, "game object type error");
    OnCreate();
}

GameObject::~GameObject()
{
    OnDestory();
}

void GameObject::OnCreate()
{
}

void GameObject::OnDestory()
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
    return GetMemberId();
}

std::string GameObject::GetName() const
{
    return G_GameObjectMgr()->GetName(m_gobj_type);
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

bool GameObject::MountChild(GameObjectSPtr gameobj)
{
    auto objid = gameobj->GetId();
    if(GameObjectIDInvalid(objid))
        return false;

    auto [_, isok] = m_childs.insert(std::make_pair(objid, gameobj));
    
    return isok;
}

std::pair<bool, GameObjectSPtr> GameObject::UnMountChild(GameObjectId gameobj_id)
{
    if (GameObjectIDInvalid(gameobj_id))
        return {false, nullptr};

    auto it = m_childs.find(gameobj_id);
    if (it == m_childs.end()) {
        return {false, nullptr};
    }

    m_childs.erase(it);
    return {true, it->second};
}

void GameObject::OnFatherDead() const
{
}

void GameObject::OnUpdate()
{
}



std::string GameObject::Dbg_GameObjectTree() const
{
    /*
    |---@type1(11)
    |---@type2(22)
        |---@type3(33)
        |---@type4(33)    
            |---@type3(33)
            |---@type4(33)
    */
    return __DbgTree(0);
}


#pragma region "私有函数"

std::string GameObject::__DbgTree(int level) const
{
    std::string info_tree = "";
    std::string prefix_str(level, '\t');
    info_tree = prefix_str + "|--@" + GetName() + "(type:" + std::to_string(m_gobj_type) + " | gid:" +std::to_string(GetMemberId()) + ")" + '\n';

    for (auto&& object_pair : m_childs )
    {
        auto gameobj = object_pair.second;
        if (gameobj) {
            info_tree += gameobj->__DbgTree(level + 1);
        }
    }

    return info_tree;
}


bool GameObject::HasGameobj(GameObjectId id) const
{
    if(GameObjectIDInvalid(id))
        return false;

    auto it = m_childs.find(id);
    if(it == m_childs.end())
        return false;

    return true;
}

void GameObject::Update()
{
    /* 预更新 */
    OnPreUpdate();

    /* 更新组件 */
    UpdateComponent();

    for(auto child_ptr : m_childs) {
        auto gobj_ptr = child_ptr.second;
        /* 递归更新子组件 */
        gobj_ptr->Update();
    }

    /* 更新完毕 */
    OnUpdate();
}

void GameObject::UpdateComponent()
{
    for (auto&& pair : m_component_map) {
        auto comp = pair.second;
        if (comp == nullptr) {
            continue;
        }

        comp->Update();
    }
}

#pragma endregion

}