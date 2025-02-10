#pragma once
#include <plugin/ecs/network/Connection.hpp>
#include "nodeserver/NodeDefine.hpp"

namespace BBTENGINE_NODE_NAMESPACE::network
{

class EchoConnection:
    public plugin::ecs::network::Connection
{
public:
    EchoConnection(bbt::network::libevent::ConnectionSPtr conn, int timeout):
    plugin::ecs::network::Connection(conn, timeout)
    {}
    ~EchoConnection() = default;
    
protected:
    virtual void OnRecv(const char* data, size_t len)
    {
        Send(data, len);
    }
};

}