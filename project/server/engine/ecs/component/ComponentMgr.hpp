#pragma once
#include "engine/ecs/EcsDefine.hpp"
#include "util/hashmap/Hashmap.hpp"
#include <optional>

namespace engine::ecs
{
inline const int ComponentHashBucketNum = 8;

// FIXME 需要测试功能
class ComponentMgr:
    public util::managerbase::ManagerBase<ComponentId, Component>
{
public:
    typedef std::tuple<std::string, ComponentTemplateId> ComponentInfo;

    ~ComponentMgr();
    static const std::unique_ptr<ComponentMgr>& GetInstance();

    const std::string& GetComponentName(ComponentTemplateId id);

    virtual Result Search(KeyType key);
    virtual bool IsExist(KeyType key);
    bool OnInitComponent(KeyType key, MemberPtr value);
    bool OnDestoryComponent(KeyType key);

public:
    // Component模板管理

    /**
     * @brief 初始化所有模板的信息
     * 
     * @param list 初始化列表
     * @return true 
     * @return false 已经初始化过了，不可以重复初始化
     */
    bool InitTemplateInfo(std::initializer_list<ComponentInfo> list);


protected:
    ComponentMgr();

    // override ManagerBase 
    virtual bool OnMemberCreate(MemberPtr member_base) override;

    virtual bool OnMemberDestory(KeyType key) override;

    virtual KeyType GenerateKey(MemberPtr member_base) override;

private:
    util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo, ComponentHashBucketNum> m_component_info_map;
    std::map<ComponentId, ComponentSPtr>    m_component_map;
};

}


