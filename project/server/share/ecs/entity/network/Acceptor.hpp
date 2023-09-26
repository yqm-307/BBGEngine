#pragma once
#include "util/network/IOThread.hpp"
#include "util/network/IPAddress.hpp"
#include <functional>

namespace share::ecs::entity::network::__detail
{
static const int ListenCallback_Min_MS = 1000; // 如果没有事件，多久触发一次

//XXX 这里放在evIOCallbacks.hpp中可能更好，因为本身就是IO回调事件
// 处理新连接的回调
void _AcceptReadCallback(evutil_socket_t listenfd, short event, void* args);

class Acceptor
{
    friend void _AcceptReadCallback(evutil_socket_t, short, void*);

public:
    typedef std::function<util::network::IOThread*()> LoadBlanceFunc;

    /* 创建listen 套接字 */
    Acceptor(std::string ip, short port);
    ~Acceptor();

    int Accept();
    int Close();


    evutil_socket_t Fd() const;
    const std::string& IP() const;
    short Port() const;

    int SetNonBlock();
    /* 将accept事件注册到event base中 */
    int RegistInEvBase(event_base*);
    void SetLoadBlance(const LoadBlanceFunc& cb);
private:
    void Init();
    void Destory();
    void Clear();
    /* 接收到新连接，处理新连接 */
    void OnAccept(int fd, const util::network::Address& peer_addr);

private:
    evutil_socket_t m_listen_fd;
    std::string     m_listen_ip;
    short           m_listen_port;
    util::network::Address    m_listen_addr;
    LoadBlanceFunc  m_load_blance_cb;
};

}// namespace end