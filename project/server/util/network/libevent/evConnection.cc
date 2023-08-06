#include "util/network/libevent/evConnection.hpp"

namespace game::util::network::ev
{

evConnection::evConnection()
    :m_sockfd(-1),
    m_status(ConnStatus::Done)
{
}


evConnection::~evConnection()
{
}

bool evConnection::IsClosed()
{
    return ( m_status == ConnStatus::Disconnected );
}

std::optional<std::string> evConnection::GetPeerIP()
{
    if(IsClosed())
        return std::nullopt;

    return m_ip;
}

std::optional<std::string> evConnection::GetLocalIP()
{
    return std::nullopt;
}

std::optional<int>  evConnection::GetPeerPort()
{
    return std::nullopt;
}

std::optional<int> evConnection::GetLocalPort()
{
    return std::nullopt;
}

size_t evConnection::Send(const char* buffer, size_t len)
{
    return 0;
}

void evConnection::Close()
{
}


}