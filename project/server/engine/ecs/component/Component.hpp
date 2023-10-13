#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>
#include "engine/ecs/EcsDefine.hpp"

namespace engine::ecs
{

class Component 
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
protected:
    virtual void OnCreate(){}
    virtual void OnDestory(){}
    virtual void SetId(ComponentId) final;
private:
    ComponentId             m_id{-1};
    ComponentTemplateId     m_template_id;
};

}