#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <plugin/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/rpc/RpcServer.hpp>
#include <plugin/ecs/rpc/RpcClient.hpp>

class DbgRpcClient:
    public plugin::ecs::rpc::RpcClient
{
    ComponentClassMetaInfo(1000001);
public:
    DbgRpcClient(engine::ecs::ComponentTemplateId id):
        RpcClient(id)
    {
    }

    void OnUpdate() {};

    int Send(const bbt::buffer::Buffer& buffer) override
    {
        return -1;
    }
};

class DbgRpcServer:
    public plugin::ecs::rpc::RpcServer
{
    ComponentClassMetaInfo(1000002);
public:
    DbgRpcServer(engine::ecs::ComponentTemplateId id):
        RpcServer(id)
    {
    }
    void OnUpdate() {};

    void Send(const char* data, size_t size) override
    {
        auto parent = GetParentObject();
        if (parent)
        {
            auto client = parent->GetComponent<DbgRpcClient>();
            if (client)
            {
                client->OnRecv(data, size);
            }
        }
    }
};

BOOST_AUTO_TEST_SUITE(RpcTest)

BOOST_AUTO_TEST_CASE(t_rpc)
{

    auto client = std::make_shared<DbgRpcClient>(plugin::ecs::EM_COMPONENT_TYPE_LUA_TEST);
    auto server = std::make_shared<DbgRpcServer>(plugin::ecs::EM_COMPONENT_TYPE_LUA_TEST);

    server->Register("test", [](const bbt::buffer::Buffer& buffer) -> util::errcode::ErrOpt {
        
        return std::nullopt;
    });

    client->Call([](const bbt::buffer::Buffer& buffer) -> util::errcode::ErrOpt {
        return std::nullopt;
    }, "test", 1, 2, 3);
}

BOOST_AUTO_TEST_SUITE_END()