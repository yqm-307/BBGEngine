#include <cluster/connection/NRConnection.hpp>
#include <cluster/registery/RegisteryBase.hpp>

namespace cluster
{

NRConnection::NRConnection(bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms)
{
}

} // namespace cluster