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
    /* 设置监听地址 */
    m_listen_addr.set(ip, port);
    auto thread = m_master_thread.lock();
    DebugAssert(thread == nullptr);
    if(thread == nullptr)
        return errcode::ErrCode("io thread is null!");

    auto [err, event_id] = thread->RegisterEventSafe([this](ev_socklen_t fd, short event, void* args){

        //TODO 参数 check
        while(true)
        {
            auto [err_accept, fd] = Accept();
            if(err_accept != std::nullopt){
                GAME_EXT1_LOG_WARN("%s, errno=%d",err_accept.value().What().c_str(), errno);
                break;
            }
        }

    }, m_listen_fd, EV_READ | EV_PERSIST, m_accept_once_timeval);
    DebugAssert(err != std::nullopt);

    return err;
}


std::pair<errcode::ErrOpt, int> Acceptor::Accept()
{
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&addr), &len);

    util::network::Address endpoint;
    endpoint.set(addr);

    if(fd >= 0) {
        m_callbacks->OnAccept(m_master_thread, fd, endpoint);
        return {std::nullopt, fd};
    }

    if (errno == EAGAIN || errno == ECONNABORTED || errno == EINTR) {
        return {errcode::ErrCode("::accept() failed! ECONNABORTED or EINTR, please try again!", 
                errcode::ErrType::NetWorkErr, 
                errcode::errenum::MODULE_NETWORK::Accept_TryAgain), -1};
    } else {
        return {errcode::ErrCode("::accept() failed! EINVAL or other errno, check socket fd!",
                errcode::ErrType::NetWorkErr, 
                errcode::errenum::Socket_BadFd), -1};
    }

}

const Address& Acceptor::GetAddress() const
{
    return m_listen_addr;
}

errcode::ErrOpt Acceptor::Stop()
{
    /* 注销事件，并释放资源 */
    auto thread = m_master_thread.lock();
    if(!thread)
        return errcode::ErrCode("io thread is null");
    
    int error = thread->UnRegisterEvent(m_event->GetEventID());

    if(error < 0)
        return errcode::ErrCode("unregist accept event failed! event not exist!");

    ::close(m_listen_fd);
    
    m_listen_addr.Clear();

    return error;
}

} // namespace detail
} // namespace util::network::ev