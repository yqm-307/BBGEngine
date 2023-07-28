#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>
#include "share/ecs/ComponentDef.hpp"

namespace game::share::ecs
{

class GameObject;

class Component: 
    virtual public bbt::templateutil::BaseType<Component>
{
public:
    Component(std::string name);


    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnAddComponent() = 0;
    virtual void OnDelComponent() = 0;
    virtual void OnUpdate() = 0;
    //--------------------------------------------

    virtual void OnCreate(){}
    virtual void OnDestory(){}
    const std::string& GetName() const;
    ComponentTemplateId GetTemplateId() const;
protected:
private:
    ComponentTemplateId     m_template_id;
    std::string             m_name;
};

}