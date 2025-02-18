#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "./DbgRpcServer.hpp"
#include "./DbgRpcClient.hpp"
#include <plugin/ecs/rpc/RpcSerializer.hpp>

plugin::ecs::rpc::RpcSerializer decoder;

BOOST_AUTO_TEST_SUITE(RpcTest)

BOOST_AUTO_TEST_CASE(t_rpc)
{
    auto gameobject = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    gameobject->AddComponent<DbgRpcServer>(ServerCompId);
    gameobject->AddComponent<DbgRpcClient>(ClientCompId);
    auto server = gameobject->GetComponent<DbgRpcServer>();
    auto client = gameobject->GetComponent<DbgRpcClient>();


    server->Register("add", [](bbt::core::Buffer& req, bbt::core::Buffer& resp)->util::errcode::ErrOpt
    {
        auto [err, params] = decoder.Deserialize(req);
        if (err != std::nullopt)
            BOOST_ERROR(err->CWhat());
        BOOST_ASSERT(params.size() == 2);
        decoder.SerializeAppend(resp, params[0].value.int32_value + params[1].value.int32_value);
        return std::nullopt;
    });

    server->Register("ping", [](bbt::core::Buffer& req, bbt::core::Buffer& resp)->util::errcode::ErrOpt
    {
        BOOST_ASSERT(req.DataSize() == 0);
        decoder.SerializeAppend(resp, "pong");
        return std::nullopt;
    });

    client->Call([](bbt::core::Buffer& buffer)->util::errcode::ErrOpt
    {
        auto [err, params] = decoder.Deserialize(buffer);
        if (err != std::nullopt)
            BOOST_ERROR(err->CWhat());
        BOOST_ASSERT(!params.empty());
        BOOST_ASSERT(params[0].value.int32_value == 30);
        return std::nullopt;
    }, "add", 10, 20);

    client->Call([](bbt::core::Buffer& buffer)->util::errcode::ErrOpt
    {
        auto [err, params] = decoder.Deserialize(buffer);
        BOOST_ASSERT(!params.empty());
        BOOST_ASSERT(params[0].string == "pong");
        return std::nullopt;
    }, "ping");

    
}

BOOST_AUTO_TEST_SUITE_END()