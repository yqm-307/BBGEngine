#include "ComponentMgr.hpp"

namespace game::share::ecs
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


}