#include "./TestComponent.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::component
{

TestComponent::TestComponent(const MyCallback& OnAdd, const MyCallback& OnDel)
    :engine::ecs::Component(share::ecs::emComponentType::EM_COMPONENT_TYPE_TESTCOMP_1)
{

}

TestComponent::~TestComponent()
{

}

}