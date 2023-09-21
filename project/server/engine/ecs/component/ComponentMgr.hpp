#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>
#include "engine/ecs/component/ComponentDef.hpp"
#include <optional>

namespace engine::ecs
{
inline const int ComponentHashBucketNum = 8;

// FIXME 需要测试功能
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


