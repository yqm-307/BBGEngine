#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/libevent/evConnMgr.hpp"
#include "gameserver/network/Acceptor.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
#include <fcntl.h>
#include <evutil.h>

namespace server::network
{

Acceptor::Acceptor(std::string ip, short port)
    :m_listen_addr(ip),
    m_listen_port(port)
{
    Assert(m_listen_port >= 0 && m_listen_port <= ((1 << 16) - 1) );
    Init();
}

Acceptor::~Acceptor()
{
    Destory();
}

void Acceptor::Init()
{
    game::util::network::CreateListen(m_listen_addr, m_listen_port, true);
    sockaddr_in addr;
    socklen_t len=0;
    int error=0;

    m_listen_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    Assert(m_listen_fd >= 0);

    addr.sin_port = ntohs(m_listen_port);
    addr.sin_family = AF_INET;

    if(m_listen_addr.empty())
        addr.sin_addr.s_addr = INADDR_ANY;  // 监听任意地址
    else
        error = evutil_inet_pton(AF_INET, m_listen_addr.c_str(), &addr.sin_addr.s_addr);   // 监听指定地址

    Assert(error >= 0);

    len = sizeof(addr);
    error = ::bind(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), len);
    Assert(error >= 0);

    error = ::listen(m_listen_fd, 5);
    Assert(error >= 0);
}

void Acceptor::Destory()
{
    ::close(m_listen_fd);
    Clear();
}

void Acceptor::Clear()
{
    m_listen_fd = -1;
    m_listen_addr = "";
    m_listen_port = -1;
}

int Acceptor::SetNonBlock()
{
    int error = game::util::network::SetFdNoBlock(m_listen_fd);
    DebugAssert(error >= 0);
    return 0;
}

evutil_socket_t Acceptor::Fd() const
{
    return m_listen_fd;
}

const std::string& Acceptor::IP() const
{
    return m_listen_addr;
}

short Acceptor::Port() const
{
    return m_listen_port;
}

int Acceptor::Accept()
{
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), &len);
    if(fd >= 0) {
        GAME_EXT1_LOG_DEBUG("accept newfd=%d", fd);
        OnAccept(fd);
        return fd;
    }
    return -1;
}

int Acceptor::Close()
{
    int error = ::close(m_listen_fd);
    if(error < 0)
    {
        GAME_BASE_LOG_WARN("::close() fatal!", error);
    }
    Clear();
    return error;
}

void _AcceptReadCallback(evutil_socket_t listenfd, short event, void* args)
{
    GAME_EXT1_LOG_DEBUG("listenfd=%d    event=%d", listenfd, event);
    auto pthis = reinterpret_cast<Acceptor*>(args);
    DebugAssert(pthis != nullptr);
    /* 取出所有新连接 */
    while(1)
    {
        int fd = pthis->Accept();
        if(fd < 0)
            break;
        else
            pthis->OnAccept(fd);
    }
    if( !(errno == EINTR ||  errno == EAGAIN || errno == ECONNABORTED) )
        GAME_BASE_LOG_ERROR("accept failed! errno=%d", errno);
}

int Acceptor::RegistInEvBase(event_base* ev_base)
{
    int error = 0;
    event* ev = nullptr;
    timeval target_interval;

    DebugAssert(ev_base);
    if(ev_base == nullptr)
        return -1;

    /* 构造event */
    ev = event_new(ev_base, m_listen_fd, EV_READ | EV_PERSIST, _AcceptReadCallback, this);
    DebugAssert(ev != nullptr);
    if(ev == nullptr) {
        GAME_BASE_LOG_WARN("call event_new() failed!");
        return -1;
    }

    evutil_timerclear(&target_interval);
    target_interval.tv_usec = ListenCallback_Min_MS * 1000;

    error = event_add(ev, NULL);
    DebugAssert(error >= 0);
    if(error < 0) {
        GAME_BASE_LOG_WARN("call event_add() failed!");
        return -1;
    }
    
    return 0;
}

void Acceptor::OnAccept(int fd)
{
    GAME_EXT1_LOG_DEBUG("accept success!");
    if(fd >= 0)
    {
        //TODO 创建新连接，并初始化IO事件。此后Connection的状态应该是自完备的，和Acceptor完全无关。
    }
}

}