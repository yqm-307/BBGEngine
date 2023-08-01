#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>
#include "share/ecs/ComponentDef.hpp"
#include "share/ecs/EcsDefine.hpp"

namespace game::share::ecs
{

class Component 
{
public:
    Component(std::string name, ComponentTemplateId id);


    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnAddComponent(ecs::GameObjectRawPtr) = 0;
    virtual void OnDelComponent(ecs::GameObjectRawPtr) = 0;
    virtual void OnUpdate() = 0;
    //--------------------------------------------

    const std::string& GetName() const;
    ComponentTemplateId GetTemplateId() const;
protected:
    virtual void OnCreate(){}
    virtual void OnDestory(){}
protected:
private:
    ComponentTemplateId     m_template_id;
    std::string             m_name;
};

}