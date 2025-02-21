#include <cluster/registery/RegisteryServer.hpp>

namespace cluster
{

RegisteryServer::RegisteryServer(RegisteryBase* base, const std::string& ip, short port, int connect_timeout):
    util::network::TcpServer(ip, port, connect_timeout),
    m_registery(base)
{
}



} // namespace cluster