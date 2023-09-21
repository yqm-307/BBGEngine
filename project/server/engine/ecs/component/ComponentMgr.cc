#include "engine/ecs/component/ComponentMgr.hpp"

namespace engine::ecs
{

ComponentMgr::ComponentMgr()
    :m_component_info_map([](const ComponentTemplateId& key)->size_t{ return (size_t)(key%ComponentHashBucketNum);},nullptr)
{
    m_component_info_map.Insert({
        std::make_pair(EM_NoneComponent,    std::make_shared<ComponentInfo>(GSComponentName.component_none_name)),
        std::make_pair(EM_AoiComponent,     std::make_shared<ComponentInfo>(GSComponentName.component_aoi_name)),
    });
}

ComponentMgr::~ComponentMgr()
{
    m_component_info_map.Clear();
}

const std::unique_ptr<ComponentMgr>& ComponentMgr::GetInstance()
{
    static std::unique_ptr<ComponentMgr> _instance = nullptr;
    if(_instance == nullptr)
        _instance = std::unique_ptr<ComponentMgr>(new ComponentMgr());
    return _instance;
}


const std::string& ComponentMgr::GetComponentName(ComponentTemplateId id)
{
    auto [obj, isok] = m_component_info_map.Find(id);
    if(!isok)
        return GSComponentName.component_none_name;
    return obj->Name;
}

bool ComponentMgr::OnInitComponent(KeyType key, ValueType value)
{
    auto [_,isok] = m_component_map.insert(std::make_pair(key, value));
    return isok;
}

bool ComponentMgr::OnDestoryComponent(KeyType key)
{
    auto it = m_component_map.find(key);
    if(it == m_component_map.end())
    {
        return false;
    }

    m_component_map.erase(it);
    return true;
}

ComponentMgr::Result ComponentMgr::Search(KeyType key)
{
    auto it_obj = m_component_map.find(key);
    if(it_obj == m_component_map.end())
        return {nullptr, false};
    
    return {it_obj->second, true};
}

bool ComponentMgr::IsExist(KeyType key)
{
    return !(m_component_map.find(key) == m_component_map.end());
}

}