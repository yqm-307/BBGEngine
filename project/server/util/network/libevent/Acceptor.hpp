#pragma once
#include <functional>

#include "util/network/IOThread.hpp"
#include "util/network/IPAddress.hpp"
#include "util/network/libevent/evEvent.hpp"
#include "util/network/INetwork.hpp"
#include "util/network/libevent/evIOCallbacks.hpp"

namespace util::network::ev
{

namespace detail
{

class Acceptor
{
public:
    Acceptor(evIOThreadWKPtr io_thread);
    ~Acceptor();

    errcode::ErrOpt     Listen(std::string ip, short port);
    int                 Stop();
    int                 Accept();

    void                SetNetCallback(evNetCallback* callback);

    /* 获取监听地址 */
    const Address&      GetAddress() const;
    short               Port() const;
private:
    evutil_socket_t     m_listen_fd{-1};
    std::string         m_listen_ip{""};
    short               m_listen_port{-1};
    evEventSPtr         m_event{nullptr};
    Address             m_listen_addr;
    int                 m_accept_once_timeval{50};  //TODO 配置文件
    evIOThreadWKPtr     m_master_thread;  // 所属的io线程
    evNetCallback*      m_callbacks;
};

} // namespace detail
} // namespace util::network::ev