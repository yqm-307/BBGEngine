#include <typeinfo>
#include <bbt/ecs/component/Component.hpp>
#include <bbt/ecs/entity/Entity.hpp>
namespace bbt::ecs
{



Entity::Entity(int gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type >= 0, "game object type error");
}

Entity::~Entity()
{
}

ComponentSPtr Entity::GetComponent(ComponentTemplateId tid) const
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
        
    return it->second;
}

size_t Entity::GetComponentCount() const
{
    return m_component_map.size();
}

ComponentSPtr Entity::DelComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
    
    m_component_map.erase(it);
    it->second->OnDelComponent(shared_from_this());
    if (it->second == nullptr) {
        return nullptr;
    }

    m_tag_set.DelTag(it->second->Reflex_GetTypeId());
    return it->second;
}


bool Entity::AddComponent(ComponentSPtr component)
{
    if(component == nullptr)
        return false;

    ComponentTemplateId tid = component->GetTemplateId();
    auto it = m_component_map.insert(std::make_pair(tid, component));
    if (!it.second) {
        return false;
    }

    component->OnAddComponent(shared_from_this());

    // 处理tag
    return m_tag_set.AddTag(Tag{component->Reflex_GetTypeId()});
}

int Entity::Type()
{
    return m_gobj_type;
}

EntityId Entity::GetId()
{
    return GetMemberId();
}

std::string Entity::GetName() const
{
    return GetEntityMgr()->GetName(m_gobj_type);
}

EntitySPtr Entity::GetGameObject(EntityId id)
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

bool Entity::MountChild(EntitySPtr gameobj)
{
    auto objid = gameobj->GetId();
    if(GameObjectIDInvalid(objid))
        return false;

    auto [_, isok] = m_childs.insert(std::make_pair(objid, gameobj));

    OnMountChild(gameobj);
    
    return isok;
}

std::pair<bool, EntitySPtr> Entity::UnMountChild(EntityId gameobj_id)
{
    if (GameObjectIDInvalid(gameobj_id))
        return {false, nullptr};

    auto it = m_childs.find(gameobj_id);
    if (it == m_childs.end()) {
        return {false, nullptr};
    }

    m_childs.erase(it);
    OnUnMountChild(it->second);

    return {true, it->second};
}

void Entity::OnFatherDead() const
{
}

void Entity::OnUpdate()
{
}



std::string Entity::Dbg_GameObjectTree() const
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

TagSet& Entity::GetTagSet()
{
    return m_tag_set;
}

SceneSPtr Entity::GetScene() const
{
    return GetEntityMgr()->GetScene();
}

EntityMgrSPtr Entity::GetEntityMgr() const
{
    return std::static_pointer_cast<EntityMgr>(GetManager());
}


#pragma region "私有函数"

std::string Entity::__DbgTree(int level) const
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


bool Entity::HasGameobj(EntityId id) const
{
    if(GameObjectIDInvalid(id))
        return false;

    auto it = m_childs.find(id);
    if(it == m_childs.end())
        return false;

    return true;
}

void Entity::Update()
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

void Entity::UpdateComponent()
{
    for (auto&& pair : m_component_map) {
        auto comp = pair.second;
        if (comp == nullptr) {
            continue;
        }

        comp->Update();
    }
}

void Entity::OnMountChild(EntitySPtr gameobj)
{
}

void Entity::OnUnMountChild(EntitySPtr gameobj)
{
}

#pragma endregion

}