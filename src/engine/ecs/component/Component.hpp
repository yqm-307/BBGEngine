#pragma once
#include <string>
#include <bbt/base/templateutil/BaseType.hpp>
#include <engine/ecs/EcsDefine.hpp>
#include "./ComponentMgr.hpp"

namespace engine::ecs
{

class Component:
    public bbt::templateutil::MemberBase<ComponentId, Component>,
    public bbt::core::reflex::ReflexDynTypeInfo<Component>
{
    friend class GameObject;
    friend class ComponentMgr;
public:
    explicit Component();

    void                        SetActive(bool is_active);
    bool                        IsActive();

    // interface
    //--------------------------------------------
    virtual                     ~Component() = 0;
    virtual void                OnUpdate() = 0;
    virtual bbt::core::reflex::TypeId Reflex_GetTypeId() = 0;
    virtual const char*         Reflex_GetTypeName() = 0;
    virtual void                OnAddComponent(ecs::GameObjectSPtr);
    virtual void                OnDelComponent(ecs::GameObjectSPtr);
    virtual void                OnEnable();
    virtual void                OnDisable();
    virtual void                Init();
    //--------------------------------------------

    const char*                 GetName();
    ComponentTemplateId         GetTemplateId();
    ComponentId                 GetId() const;
    GameObjectSPtr              GetParentObject() const;
    SceneSPtr                   GetScene() const;
    ComponentMgrSPtr            GetComponentMgr() const;
protected:
    virtual void OnCreate();
    virtual void OnDestory();
private:
    virtual void Update() final;
private:
    bool                    m_is_active;
    /**
     * 父对象的弱引用
     */
    GameObjectWKPtr         m_parent_gameobject;
};

}