#pragma once
#include <plugin/ecs/network/ServerComp.hpp>
#include <nodeserver/ecs/network/EchoConnection.hpp>

namespace BBTENGINE_NODE_NAMESPACE::network
{

class EchoService:
    public plugin::ecs::network::Server
{
    ComponentDeriveClassDef;
protected:
    EchoService(const std::string& ip, short port, int connect_timeout):
        plugin::ecs::network::Server(ip, port, connect_timeout)
    {}

    virtual std::shared_ptr<plugin::ecs::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override
    {
        return std::make_shared<EchoConnection>(conn, 4000);
    }
};

}