#pragma once
#include <string>
#include <bbt/ecs/Define.hpp>
#include "./ComponentMgr.hpp"

namespace bbt::ecs
{

class Component:
    public bbt::core::util::MemberBase<ComponentId, Component>,
    public bbt::core::reflex::ReflexDynTypeInfo<Component>
{
    friend class Entity;
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
    virtual void                OnAddComponent(ecs::EntitySPtr);
    virtual void                OnDelComponent(ecs::EntitySPtr);
    virtual void                OnEnable();
    virtual void                OnDisable();
    virtual void                Init();
    //--------------------------------------------

    const char*                 GetName();
    ComponentTemplateId         GetTemplateId();
    ComponentId                 GetId() const;
    EntitySPtr                  GetParentObject() const;
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
    EntityWKPtr         m_parent_gameobject;
};

}