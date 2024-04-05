#pragma once
#include <string>
#include <bbt/base/templateutil/BaseType.hpp>
#include "./ComponentMgr.hpp"

namespace engine::ecs
{

class Component:
    public bbt::templateutil::MemberBase<ComponentId, Component>
{
    friend class GameObject;
    friend class ComponentMgr;
public:
    explicit Component(ComponentTemplateId id);

    void SetActive(bool is_active);
    bool IsActive();

    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnAddComponent(ecs::GameObjectSPtr);
    virtual void OnDelComponent(ecs::GameObjectSPtr);
    virtual void OnEnable();
    virtual void OnDisable();
    //--------------------------------------------

    const std::string& GetName() const;
    ComponentTemplateId GetTemplateId() const;
    ComponentId GetId() const;
protected:
    virtual void OnCreate();
    virtual void OnDestory();
private:
    virtual void Update() final;
private:
    ComponentTemplateId     m_template_id;
    bool                    m_is_active;
};

}