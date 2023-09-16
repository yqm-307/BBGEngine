#include "ComponentMgr.hpp"

namespace game::share::ecs
{

ComponentMgr::ComponentMgr()
    :m_component_info_map([](const ComponentTemplateId& key)->size_t{ return (size_t)(key%ComponentHashBucketNum);},nullptr)
{
    m_component_info_map.Insert({
        std::make_pair(EM_AoiComponent, std::make_shared<ComponentInfo>("AoiComponent")),
    });
}

ComponentMgr::~ComponentMgr()
{
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
    // FIXME 需要换解决方法
    if(!isok)
        return "";
    return obj->Name;
}


}

// #ifdef ComponentHashBucketNum
// #undef ComponentHashBucketNum
// #endif