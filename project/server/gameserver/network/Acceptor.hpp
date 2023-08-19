#pragma once
#include "util/network/IOCallbacks.hpp"
#include <functional>

namespace server::network
{
static const int ListenCallback_Min_MS = 1000; // 如果没有事件，多久触发一次

void _AcceptReadCallback(evutil_socket_t listenfd, short event, void* args);

class Acceptor
{
public:
    friend void _AcceptReadCallback(evutil_socket_t, short, void*);
    /* 创建listen 套接字 */
    Acceptor(std::string ip, short port);
    ~Acceptor();

    int Accept();
    int Close();


    evutil_socket_t Fd() const;
    const std::string& IP() const;
    short Port() const;

    int SetNonBlock();
    int AddInEventBase(event_base*);

private:
    void Init();
    void Destory();
    void Clear();
    void OnAccept(int fd);

private:
    evutil_socket_t m_listen_fd;
    std::string     m_listen_addr;
    short           m_listen_port;
};

}// namespace end