#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>
#include "./ComponentMgr.hpp"

namespace engine::ecs
{

class Component:
    public util::managerbase::MemberBase<ComponentId, Component>
{
public:
    explicit Component(ComponentTemplateId id);


    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnAddComponent(ecs::GameObjectSPtr) = 0;
    virtual void OnDelComponent(ecs::GameObjectSPtr) = 0;
    virtual void OnUpdate() = 0;
    //--------------------------------------------

    const std::string& GetName() const;
    ComponentTemplateId GetTemplateId() const;
    ComponentId GetId() const;
protected:
    virtual void OnCreate();
    virtual void OnDestory(){}
private:
    ComponentTemplateId     m_template_id;
};

}