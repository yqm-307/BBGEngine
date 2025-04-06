#include "bbt/ecs/component/Component.hpp"
#include "bbt/ecs/component/ComponentMgr.hpp"

namespace bbt::ecs
{

ComponentMgr::ComponentMgr(SceneSPtr scene):
    m_scene(scene)
{
}

ComponentMgr::~ComponentMgr()
{
}

ComponentMgr::Result ComponentMgr::Search(KeyType key)
{
    auto it_obj = m_component_map.find(key);
    if(it_obj == m_component_map.end())
        return {nullptr, false};
    
    return {(it_obj->second).lock(), true};
}

bool ComponentMgr::IsExist(KeyType key)
{
    return !(m_component_map.find(key) == m_component_map.end());
}

bool ComponentMgr::OnMemberCreate(MemberPtr member_base)
{
    auto comp_id = member_base->GetMemberId();
    auto it = m_component_map.find(comp_id);
    if(it != m_component_map.end())
        return false;
    
    auto [_, isok] = m_component_map.insert(std::make_pair(comp_id, member_base));
    return isok;
}

bool ComponentMgr::OnMemberDestory(KeyType key)
{
    auto it = m_component_map.find(key);
    if(it == m_component_map.end())
        return false;
    
    auto idx = m_component_map.erase(key);
    return (idx > 0);
}

ComponentMgr::KeyType ComponentMgr::GenerateKey(MemberPtr member)
{
    return bbt::ecs::GenerateComponentID();
}

SceneSPtr ComponentMgr::GetScene() const
{
    return m_scene.lock();
}

}