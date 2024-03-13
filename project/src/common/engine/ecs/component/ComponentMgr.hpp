#pragma once
#include "engine/ecs/EcsDefine.hpp"
#include "util/hashmap/Hashmap.hpp"
#include <optional>

namespace engine::ecs
{
inline const int ComponentHashBucketNum = 8;

// FIXME 需要测试功能
class ComponentMgr:
    public bbt::templateutil::ManagerBase<ComponentId, Component>
{
public:
    typedef std::pair<MemberPtr, bool> Result;
    typedef std::tuple<std::string, ComponentTemplateId> ComponentInfo;

    ~ComponentMgr();
    static const std::unique_ptr<ComponentMgr>& 
                        GetInstance();
    const std::string&  GetComponentName(ComponentTemplateId id);
    virtual Result      Search(KeyType key);
    virtual bool        IsExist(KeyType key);
public:
    bool                InitTemplateInfo(std::initializer_list<ComponentInfo> list);
protected:
    ComponentMgr();

    virtual bool        OnMemberCreate(MemberPtr member_base) override;
    virtual bool        OnMemberDestory(KeyType key) override;
    virtual KeyType     GenerateKey(MemberPtr member_base) override;

private:
    /* 组件信息表 */
    util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo, ComponentHashBucketNum> 
                        m_component_info_map;
    /* 全局组件表 */
    std::map<ComponentId, ComponentWKPtr>
                        m_component_map;
};

}


