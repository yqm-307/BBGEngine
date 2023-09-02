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
    :m_listen_ip(ip),
    m_listen_port(port),
    m_listen_addr(ip, port)
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
    m_listen_fd = game::util::network::CreateListen(m_listen_ip, m_listen_port, true);
    Assert(m_listen_fd >= 0);
}

void Acceptor::Destory()
{
    ::close(m_listen_fd);
    Clear();
}

void Acceptor::Clear()
{
    m_listen_fd = -1;
    m_listen_ip = "";
    m_listen_port = -1;
}

int Acceptor::SetNonBlock()
{
    int error = game::util::network::SetFdNoBlock(m_listen_fd);
    DebugAssert(error >= 0);
    return 0;
}

void Acceptor::SetLoadBlance(const LoadBlanceFunc& cb)
{
    DebugAssert(cb != nullptr);
    m_load_blance_cb = cb;
}

evutil_socket_t Acceptor::Fd() const
{
    return m_listen_fd;
}

const std::string& Acceptor::IP() const
{
    return m_listen_ip;
}

short Acceptor::Port() const
{
    return m_listen_port;
}

int Acceptor::Accept(sockaddr* addr, socklen_t* len)
{
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), &len);
    game::util::network::Address endpoint;
    endpoint.set(addr);
    if(fd >= 0) {
        GAME_EXT1_LOG_DEBUG("[Acceptor::Accept] Acceptor ==> ip:{%s} fd:{%d}", endpoint.GetIPPort().c_str(), fd);
        OnAccept(fd, endpoint);
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
    GAME_EXT1_LOG_DEBUG("[_AcceptReadCallback] listenfd=%d event=%d", listenfd, event);
    auto pthis = reinterpret_cast<Acceptor*>(args);
    DebugAssert(pthis != nullptr);
    /* 取出所有新连接 */
    while(1)
    {
        int fd = pthis->Accept();
        if(fd < 0)
            break;
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

void Acceptor::OnAccept(int fd, const game::util::network::Address& peer_addr)
{
    GAME_EXT1_LOG_DEBUG("accept success!");
    if(fd >= 0)
    {
        //一、 创建新连接，并初始化IO事件。此后Connection的状态应该是自完备的，和Acceptor完全无关。
        DebugAssertWithInfo(m_load_blance_cb != nullptr, "loadblance callback not initialized!");
        auto run_in_target_thread = m_load_blance_cb();
        DebugAssertWithInfo(run_in_target_thread, "loadblance error!");
        auto new_conn = game::util::network::ev::evConnMgr::GetInstance()->CreateConn(run_in_target_thread, fd, peer_addr, m_listen_addr);

        GAME_BASE_LOG_INFO("[Acceptor::OnAccept] Acceptor ==> Client IP:{%s}", peer_addr.GetIPPort().c_str());
    }
}

}