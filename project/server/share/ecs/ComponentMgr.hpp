#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>
#include "share/ecs/ComponentDef.hpp"
#include <optional>

#define ComponentHashBucketNum 8


namespace game::share::ecs
{

class ComponentMgr
{
public:
    static const std::unique_ptr<ComponentMgr>& GetInstance();
    ~ComponentMgr();

    const std::string& GetComponentName(ComponentTemplateId id);
private:
    ComponentMgr();
private:
    util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo::SPtr, ComponentHashBucketNum> m_component_info_map;
};




}


