#include "util/network/libevent/evConnection.hpp"
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/log/Log.hpp"

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

const event_base* evConnection::GetEvBase() const
{
    return m_ev_base;
}

void evConnection::InitEvent()
{
    int err = 0;
    m_ev_event = event_new(m_ev_base, m_sockfd, EV_PERSIST | EV_READ, OnRecvCallback, NULL);
    err = event_add(m_ev_event, nullptr);
    if(err < 0)
    {
        GAME_EXT1_LOG_ERROR("event add failed!");
    }
}

void evConnection::OnRecv(int sockfd)
{

}

}