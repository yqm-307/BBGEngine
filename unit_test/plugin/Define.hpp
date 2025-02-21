#pragma once
#include <engine/ecs/entity/Entity.hpp>
#include <plugin/scene/testscene/SampleScene.hpp>
#include <plugin/ecs/rpc/RpcServer.hpp>
#include <plugin/ecs/rpc/RpcClient.hpp>
#include <plugin/ecs/entity/Entity.hpp>

#define ClientCompId 1000001
#define ServerCompId 1000002

class DbgRpcClient;
class DbgRpcServer;