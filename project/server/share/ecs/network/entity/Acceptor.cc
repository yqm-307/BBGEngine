#include "util/network/libevent/evConnMgr.hpp"
#include "share/ecs/network/entity/Acceptor.hpp"
#include "util/network/libevent/evIOThread.hpp"
#include <fcntl.h>
#include <evutil.h>

namespace share::ecs::entity::network::__detail
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
    m_listen_fd = util::network::CreateListen(m_listen_ip, m_listen_port, true);
    Assert(m_listen_fd >= 0);

    m_event = util::network::ev::evEvent::Create([this](ev_socklen_t fd, short event, void* args){
        AcceptEvent(fd, event, args); 
    }, m_listen_fd, EV_READ | EV_PERSIST, m_accept_once_timeval);
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

int Acceptor::DoAccept()
{
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), &len);

    util::network::Address endpoint;
    endpoint.set(addr);

    if(fd >= 0) {
        OnAccept(fd, endpoint);
        return fd;
    }
    return -1;
}

int Acceptor::Stop()
{
    /* 注销事件，并释放资源 */
    auto thread = m_master_thread.lock();
    if(!thread)
        return -1;
    
    int error = thread->UnRegisterEvent(m_event->GetEventID());

    if(error < 0)
        return -1;

    error = ::close(m_listen_fd);
    if(error < 0)
        return -1;

    Clear();
    return error;
}

void Acceptor::AcceptEvent(ev_socklen_t fd, short event, void* args)
{
    while(true)
    {
        int fd = DoAccept();
        if(fd < 0)
            break;
    }

    if( !(errno == EINTR ||  errno == EAGAIN || errno == ECONNABORTED) )
        GAME_EXT1_LOG_ERROR("accept() failed! errno=%d", errno);
    
}

int Acceptor::Start(util::network::ev::evIOThreadSPtr thread)
{
    DebugAssert(thread != nullptr);
    if(thread == nullptr)
        return -1;

    m_master_thread = thread;

    int err = thread->RegisterEventSafe(m_event);
    DebugAssert(err >= 0);

    return err;
}

void Acceptor::OnAccept(int fd, const util::network::Address& peer_addr)
{
    //一、 创建新连接，并初始化IO事件。此后Connection的状态应该是自完备的，和Acceptor完全无关。
    DebugAssertWithInfo(m_load_blance_cb != nullptr, "loadblance callback not initialized!");
    auto run_in_target_thread = m_load_blance_cb();
    DebugAssertWithInfo(run_in_target_thread, "loadblance error!");
    auto new_conn = util::network::ev::evConnMgr::GetInstance()->Create<util::network::ev::evConnection>(run_in_target_thread, fd, peer_addr, m_listen_addr);
    //TODO 回调连接对象
    GAME_BASE_LOG_INFO("[Acceptor::OnAccept] Acceptor ==> Client IP:{%s}", peer_addr.GetIPPort().c_str());
    DebugAssertWithInfo(m_onconnect_cb == nullptr , "onconnect callback is null!");
    if(m_onconnect_cb)
        m_onconnect_cb(new_conn);
}

void Acceptor::SetOnConnect(const OnAcceptCallback& cb)
{
    DebugAssertWithInfo(m_onconnect_cb == nullptr, "repeat regist event!");
    m_onconnect_cb = cb;
}

}