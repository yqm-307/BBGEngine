#include "./TestComponent.hpp"
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::component
{

TestComponent::TestComponent(const MyCallback& OnAdd, const MyCallback& OnDel)
    :engine::ecs::Component(EM_COMPONENT_TYPE_TESTCOMP_1)
{

}

TestComponent::~TestComponent()
{

}

}