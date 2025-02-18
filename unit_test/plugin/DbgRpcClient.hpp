#pragma once
#include "./Define.hpp"

class DbgRpcClient:
    public plugin::ecs::rpc::RpcClient
{
    friend class DbgRpcServer;
    ComponentClassMetaInfo(ClientCompId);
public:
    DbgRpcClient(engine::ecs::ComponentTemplateId id):
        RpcClient(id)
    {
    }

    void OnUpdate() {};

    int Send(const bbt::core::Buffer& buffer) override;
};
