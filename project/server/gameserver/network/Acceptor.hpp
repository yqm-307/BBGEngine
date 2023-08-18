#pragma once
#include "util/network/IOCallbacks.hpp"

namespace server::network
{


class Acceptor
{
public:
    Acceptor(std::string ip, short port);
    ~Acceptor();


    int SetNonBlock();
private:
    void Init();
    void Destory();

private:
    evutil_socket_t m_listen_fd;
    std::string     m_listen_addr;
    short           m_listen_port;
};

}// namespace end