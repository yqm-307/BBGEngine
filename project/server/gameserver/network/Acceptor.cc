#include "gameserver/network/Acceptor.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
#include <fcntl.h>

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
}

void Acceptor::Destory()
{

}

int Acceptor::SetNonBlock()
{
    Assert(m_listen_fd >= 0);
    int old_flags = ::fcntl(m_listen_fd, F_GETFL);

    DebugAssertWithInfo( old_flags >= 0 , "maybe listen fd need init!");

    if(old_flags < 0) 
    {
        GAME_BASE_LOG_WARN("fcntl F_GETFL failed! fd=%d", m_listen_fd);        
        return -1;
    }

    int new_flags = old_flags | O_NONBLOCK;
    int error = ::fcntl(m_listen_fd, F_SETFL, &new_flags);

    DebugAssertWithInfo(error >= 0, "maybe system error");

    if(error < 0)
    {
        GAME_BASE_LOG_WARN("fcntl F_SETFL failed! fd=%d", m_listen_fd);
        return -1;
    }

    return 0;
}

}