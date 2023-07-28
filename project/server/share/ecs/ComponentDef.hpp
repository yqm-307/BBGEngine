#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>

#define ComponentHashBucketNum 8


namespace game::share::ecs
{

enum ComponentTemplateId : int32_t
{
    EM_AoiComponent    =   10001,
};



struct ComponentInfo:
    public bbt::templateutil::BaseType<ComponentInfo>
{
    ComponentInfo() = default;
    ComponentInfo(std::string name):Name(name){}
    std::string Name{""};
};



/**
 * @brief 此 hashmap 保存的是 templateid -> component info
 *  CompomentInfo较小，直接保存值 
 */
static const util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo::SPtr, ComponentHashBucketNum> ComponentInfoMap
(
[](const ComponentTemplateId& key)->size_t
{ 
    return (size_t)(key%ComponentHashBucketNum);
},
nullptr,
{
    std::make_pair(EM_AoiComponent, std::make_shared<ComponentInfo>("AoiComponent"))
});

/**
 * @brief 根据组件模板id获取组件信息，如果id不存在，返回默认值
 * 
 * @param id 模板id
 * @return ComponentInfo 组件信息 
 */
inline ComponentInfo GetComponentInfo(ComponentTemplateId id)
{
    auto [val, isexist] = ComponentInfoMap.Find(id);
    if(isexist)
        return *val;
    
    return ComponentInfo();
}

/**
 * @brief 根据组件模板id获取组件名，如果id不存在，返回默认值（查看 ComponentInfo 默认构造函数）
 * 
 * @param id 模板id
 * @return std::string 组件名
 */
inline std::string GetComponentName(ComponentTemplateId id)
{
    return GetComponentInfo(id).Name;
}





}


#ifdef ComponentHashBucketNum
#undef ComponentHashBucketNum
#endif
