#pragma once
#include <util/network/Connection.hpp>

namespace cluster
{

class RegisteryBase;

/**
 * @brief NRConnection
 * 节点和注册中心的连接
 */
class NRConnection final:
    public util::network::Connection
{
public:
    NRConnection(bbt::network::libevent::ConnectionSPtr conn, int timeout_ms);

    virtual ~NRConnection() = default;    
private:
    bbt::core::Buffer m_recv_buffer;    // 接收缓冲区
};

} // namespace cluster