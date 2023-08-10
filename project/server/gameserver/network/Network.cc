#include "gameserver/network/Network.hpp"

namespace server::network
{

Network::Network(const std::string& ip, short port)
    :m_listen_ip(ip),
    m_listen_port(port)
{
}

Network::~Network() {}


}// namespace end