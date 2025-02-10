#pragma once
#include "engine/ecs/component/Component.hpp"
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::component
{

class EmptyComponent:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    typedef std::function<void(engine::ecs::GameObjectSPtr)> MyCallback; 

    ~EmptyComponent() {} 
protected:
    EmptyComponent(): engine::ecs::Component(EM_COMPONENT_TYPE_EMPTY) {}
    virtual void OnUpdate() {}
private:

};

}