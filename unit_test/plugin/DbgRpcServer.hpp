#pragma once
#include "./Define.hpp"

class DbgRpcClient;

class DbgRpcServer:
    public plugin::ecs::rpc::RpcServer
{
    ComponentClassMetaInfo(ServerCompId);
public:
    DbgRpcServer(engine::ecs::ComponentTemplateId id):
        RpcServer(id)
    {
    }
    void OnUpdate() {};

    void Send(const char* data, size_t size) override;
};