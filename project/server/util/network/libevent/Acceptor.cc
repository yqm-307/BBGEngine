#include "Acceptor.hpp"
#include "evIOThread.hpp"

namespace util::network::ev
{

namespace detail
{

Acceptor::Acceptor(evIOThreadWKPtr io_thread)
    :m_master_thread(io_thread)
{
}

Acceptor::~Acceptor() {}

void Acceptor::SetNetCallback(evNetCallback* callback)
{
    DebugAssert(callback != nullptr);
    
}

errcode::ErrOpt Acceptor::Listen(std::string ip, short port)
{
    m_listen_addr.set(ip, port);

    auto thread = m_master_thread.lock();
    
    if(thread == nullptr)
        return std::nullopt;

    auto [err, event_id] = thread->RegisterEventSafe([this](ev_socklen_t fd, short event, void* args){
        while(true)
        {
            int fd = Accept();
            if(fd < 0)
                break;
        }

        if( !(errno == EINTR ||  errno == EAGAIN || errno == ECONNABORTED) )
            GAME_EXT1_LOG_ERROR("accept() failed! errno=%d", errno);
    }, m_listen_fd, EV_READ | EV_PERSIST, m_accept_once_timeval);
    DebugAssert(err != std::nullopt);

    return err;
}


int Acceptor::Accept()
{
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), &len);

    util::network::Address endpoint;
    endpoint.set(addr);

    if(fd >= 0) {
        m_callbacks->OnAccept(m_master_thread, fd, endpoint);
        return fd;
    }
    return -1;
}

const Address& Acceptor::GetAddress() const
{
    return m_listen_addr;
}


} // namespace detail
} // namespace util::network::ev