#pragma once
#include <functional>
#include <engine/ecs/component/Component.hpp>
#include <plugin/ecs/Define.hpp>

namespace plugin::ecs::component
{

class TestComponent:
    public engine::ecs::Component
{
    ComponentClassMetaInfo(EM_COMPONENT_TYPE_TESTCOMP_1);
public:
    typedef std::function<void(engine::ecs::EntitySPtr)> MyCallback; 

    ~TestComponent();    
protected:
    TestComponent(const MyCallback& OnAdd, const MyCallback& OnDel);
    virtual void OnUpdate() {}
private:

};

}