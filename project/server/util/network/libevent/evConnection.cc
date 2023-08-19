#include "util/network/libevent/evConnection.hpp"
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace game::util::network::ev
{



evConnection::evConnection(event_base* ev_base, int newfd, std::string peer_ip, short peer_port)
    :m_ev_base(ev_base),
    m_sockfd(newfd),
    m_ip(peer_ip),
    m_port(peer_port)
{
}


evConnection::~evConnection()
{
    Close();
    Destroy();
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
    timeval heart;

    /* 初始化网络读事件 */
    m_recv_event = event_new(m_ev_base, m_sockfd, EV_PERSIST | EV_READ, OnRecvCallback, this);
    err = event_add(m_recv_event, nullptr);
    if(err < 0) {
        GAME_EXT1_LOG_ERROR("event add failed!");
    }

    /* 初始化心跳事件 */
    evutil_timerclear(&heart);
    heart.tv_usec = evConnection_Timeout_MS * 1000;
    m_timeout_event = event_new(m_ev_base, m_sockfd, EV_PERSIST | EV_READ, OnHeartBeatCallback, this);
    err = event_add(m_timeout_event, &heart);
    if(err < 0) {
        GAME_EXT1_LOG_ERROR("event add failed!");
    }
}

void evConnection::OnRecv(int sockfd)
{
    char* buffer[40960];
    memset(buffer, '\0', sizeof(buffer));
    int nbyte = ::recv(sockfd, buffer, sizeof buffer, 0);
    if(nbyte > 0) {
        GAME_BASE_LOG_INFO("recv success! recv view: [%s]", buffer);
    }
    else if(nbyte == 0) {
        GAME_BASE_LOG_WARN("need close connection!");
    }
    else {
        GAME_BASE_LOG_ERROR("recv() failed, errno=%d", errno);
    }

}

void evConnection::Init()
{
    [[maybe_unused]] int error = 0;
    DebugAssert(m_ev_base != nullptr);
    /* 初始化心跳、读事件 */
    InitEvent();
}

void evConnection::Destroy()
{   
    [[maybe_unused]] int error = 0;

    DebugAssert(m_ev_base != nullptr);
    DebugAssert(m_recv_event != nullptr);
    m_ev_base = nullptr;    // 生命期控制权不在这里

    error = event_del(m_recv_event);
    DebugAssert(error >= 0);

    error = event_del(m_timeout_event);
    DebugAssert(error >= 0);

    event_free(m_recv_event);
    m_recv_event = nullptr;

    event_free(m_timeout_event);
    m_timeout_event = nullptr;

}

}