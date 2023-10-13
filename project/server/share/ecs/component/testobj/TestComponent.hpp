#pragma once
#include "engine/ecs/component/Component.hpp"
#include <functional>

namespace share::ecs::component
{

class TestComponent:
    engine::ecs::Component
{
public:
    typedef std::function<void(engine::ecs::GameObjectSPtr)> MyCallback; 

    TestComponent(const MyCallback& OnAdd, const MyCallback& OnDel);
    ~TestComponent();    
private:

};

}