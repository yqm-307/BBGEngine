#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>
#include "engine/ecs/ComponentDef.hpp"
#include "engine/ecs/EcsDefine.hpp"

namespace game::share::ecs
{

class Component 
{
public:
    explicit Component(std::string name, ComponentTemplateId id);


    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnAddComponent(ecs::GameObjectSPtr) = 0;
    virtual void OnDelComponent(ecs::GameObjectSPtr) = 0;
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