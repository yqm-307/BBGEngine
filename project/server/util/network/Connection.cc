#include "util/network/Connection.hpp"

namespace util::network
{

Connection::Connection(int sockfd, const Address& peer, const Address& local)
    :m_sockfd(sockfd),
    m_peer_addr(peer),
    m_local_addr(local)
{

}

Connection::~Connection()
{
    GAME_EXT1_LOG_INFO("[Connection::~Connection] release connection! sockfd=%d", GetSocket());
}

int Connection::GetSocket() const
{
    return m_sockfd;
}

const Address& Connection::GetLocalIPAddress() const
{
    return m_local_addr;
}

const Address& Connection::GetPeerIPAddress() const
{
    return m_peer_addr;
}



}