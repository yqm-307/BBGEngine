#pragma once
#include "util/network/IOThread.hpp"
#include "util/network/IPAddress.hpp"
#include <functional>

namespace server::network
{
static const int ListenCallback_Min_MS = 1000; // 如果没有事件，多久触发一次

//XXX 这里放在evIOCallbacks.hpp中可能更好，因为本身就是IO回调事件
// 处理新连接的回调
void _AcceptReadCallback(evutil_socket_t listenfd, short event, void* args);

class Acceptor
{
    friend void _AcceptReadCallback(evutil_socket_t, short, void*);

public:
    typedef std::function<game::util::network::IOThread*()> LoadBlanceFunc;

    /* 创建listen 套接字 */
    Acceptor(std::string ip, short port);
    ~Acceptor();

    int Accept(sockaddr* addr, socklen_t* len);
    int Close();


    evutil_socket_t Fd() const;
    const std::string& IP() const;
    short Port() const;

    int SetNonBlock();
    /* 将accept事件注册到event base中 */
    int RegistInEvBase(event_base*);

private:
    void Init();
    void Destory();
    void Clear();
    /* 接受到新连接，fd为新accept到的套接字 */
    void OnAccept(int fd, game::util::network::Address addr);

private:
    evutil_socket_t m_listen_fd;
    std::string     m_listen_addr;
    short           m_listen_port;
};

}// namespace end