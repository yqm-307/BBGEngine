#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <engine/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/rpc/RpcServer.hpp>
#include <plugin/ecs/rpc/RpcClient.hpp>

#define ClientCompId 1000001
#define ServerCompId 1000002

class DbgRpcClient:
    public plugin::ecs::rpc::RpcClient
{
    ComponentClassMetaInfo(ClientCompId);
public:
    DbgRpcClient(engine::ecs::ComponentTemplateId id):
        RpcClient(id)
    {
    }

    void OnUpdate() {};

    int Send(const bbt::buffer::Buffer& buffer) override
    {
        auto father = GetParentObject();
        auto comp = father->GetComponent<DbgRpcServer>();
        if (comp == nullptr)
            return -1;

        auto err = comp->OnRpc(buffer);
        
    }
};

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
    auto gameobject = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    auto cli_comp = G_ComponentMgr()->Create<DbgRpcServer>(ServerCompId);
    auto srv_comp = G_ComponentMgr()->Create<DbgRpcClient>(ClientCompId);

    BOOST_ASSERT(gameobject->AddComponent<DbgRpcServer>(ServerCompId));
    BOOST_ASSERT(gameobject->AddComponent<DbgRpcClient>(ClientCompId));

}

BOOST_AUTO_TEST_SUITE_END()