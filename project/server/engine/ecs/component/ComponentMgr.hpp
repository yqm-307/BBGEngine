#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>
#include "engine/ecs/component/Component.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include <optional>

namespace engine::ecs
{
inline const int ComponentHashBucketNum = 8;

// FIXME 需要测试功能
class ComponentMgr:
    public util::managerbase::ManagerBase<>
{
public:
    ~ComponentMgr();
    static const std::unique_ptr<ComponentMgr>& GetInstance();
    
    static ComponentSPtr Create();

    const std::string& GetComponentName(ComponentTemplateId id);
private:
    ComponentMgr();
private:
    util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo::SPtr, ComponentHashBucketNum> m_component_info_map;
};




}


