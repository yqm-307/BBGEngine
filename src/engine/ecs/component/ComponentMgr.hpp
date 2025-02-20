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

    ComponentMgr(SceneSPtr scene);
    ~ComponentMgr();
    const std::string&  GetComponentName(ComponentTemplateId id);
    virtual Result      Search(KeyType key);
    virtual bool        IsExist(KeyType key);

    template<typename TComponent>
    void                Register(const std::string& component_name);
    template<typename TComponent>
    bbt::core::reflex::TypeId GetType();
    template<typename TComponent>
    int                 GetClassName(std::string& component_name);
    SceneSPtr           GetScene() const;
    void                Update() {}
public:
    bool                InitTemplateInfo(std::initializer_list<ComponentInfo> list);
protected:

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
    SceneWKPtr          m_scene;
};


template<typename TComponent>
bbt::core::reflex::TypeId ComponentMgr::GetType()
{
    return BBT_REFLEX_GET_TYPEID(TComponent);
}

template<typename TComponent>
void ComponentMgr::Register(const std::string& component_name)
{
    bbt::core::reflex::ReflexInfoMgr::GetInstance()->Register<TComponent>(component_name);
}

template<typename TComponent>
int ComponentMgr::GetClassName(std::string& component_name)
{
    component_name = BBT_REFLEX_GET_TYPENAME(TComponent);
    return component_name.empty() ? -1 : 0;
}


}


