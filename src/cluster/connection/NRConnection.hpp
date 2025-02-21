#pragma once
#include <util/network/Connection.hpp>

namespace cluster
{

/**
 * @brief NRConnection
 * 节点和注册中心的连接
 */
class NRConnection:
    public util::network::Connection
{
public:
    NRConnection(bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
        util::network::Connection(conn, timeout_ms)
    {
    }
};

} // namespace cluster