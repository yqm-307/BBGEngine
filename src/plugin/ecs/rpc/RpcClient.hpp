#pragma once
#include <engine/ecs/component/Component.hpp>
#include <plugin/ecs/Define.hpp>

namespace plugin::ecs::rpc
{

class RpcClient:
    public engine::ecs::Component
{
    ComponentClassMetaInfo(plugin::ecs::EM_COMPONENT_TYPE_RPC_CLIENT);
public:
private:
    RpcClient();
};

} // namespace plugin::ecs::rpc