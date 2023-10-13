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
    public util::managerbase::ManagerBase<ComponentId, ComponentSPtr>
{
public:
    typedef std::tuple<std::string, ComponentTemplateId> ComponentInfo;

    ~ComponentMgr();
    static const std::unique_ptr<ComponentMgr>& GetInstance();
    
    template<typename GameObjectChildType, typename ...InitArgs>
    ComponentSPtr Create(InitArgs ...args);
    const std::string& GetComponentName(ComponentTemplateId id);

    virtual Result Search(KeyType key);
    virtual bool IsExist(KeyType key);
    bool OnInitComponent(KeyType key, ValueType value);                
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

private:
    util::hashmap::Hashmap<ComponentTemplateId, ComponentInfo, ComponentHashBucketNum> m_component_info_map;
    std::map<ComponentId, ComponentSPtr>    m_component_map;
};


template<typename GameObjectChildType, typename ...InitArgs>
ComponentSPtr ComponentMgr::Create(InitArgs ...args)
{
    static_assert( std::is_base_of_v<Component, GameObjectChildType> );

    auto sptr = std::make_shared<GameObjectChildType>(args...);
    ComponentId id = GenerateComponentID();
    sptr->SetId(id);
    auto isok = OnInitComponent(id, sptr);
    DebugAssert(isok);
    if(!isok) {
        sptr = nullptr;
        return nullptr;
    }

    return sptr;
}

}


