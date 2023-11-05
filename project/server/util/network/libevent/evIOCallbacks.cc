#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
#include <fcntl.h>
#include <evutil.h>

namespace util::network
{

[[maybe_unused]] void OnConnectCallback(evutil_socket_t sockfd, short events, void* args)
{
}

[[maybe_unused]] void OnAcceptCallback(evutil_socket_t sockfd, short events, void* args)
{
}


[[maybe_unused]] void OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args)
{
    GAME_EXT1_LOG_DEBUG("debug heart beat! sockfd=%d events=%d", sockfd, events);
}

[[maybe_unused]] evutil_socket_t CreateListen(std::string ip, short port, bool noblock)
{
    //TODO 正式版本可能需要去掉如此严格的断言，切换为异常处理。需要注意失败后套接字等资源回收
    evutil_socket_t fd = -1;
    sockaddr_in addr;
    socklen_t len=0;
    int error=0;

    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    Assert(fd >= 0);

    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if(ip.empty())
        addr.sin_addr.s_addr = INADDR_ANY;  // 监听任意地址
    else
        error = evutil_inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);   // 监听指定地址
    Assert(error >= 0);

    error = SetFdReUseAddr(fd);
    Assert(error >= 0);

    len = sizeof(addr);
    error = ::bind(fd, reinterpret_cast<sockaddr*>(&addr), len);
    Assert(error >= 0);

    if( noblock ) {
        error = SetFdNoBlock(fd);
        if(error < 0)
            return -1;
    }

    error = ::listen(fd, 5);
    Assert(error >= 0);

    return fd;
}

int SetFdNoBlock(int fd)
{
    Assert(fd >= 0);
    int old_flags = ::fcntl(fd, F_GETFL);

    DebugAssertWithInfo( old_flags >= 0 , "maybe listen fd need init!");

    if(old_flags < 0) 
    {
        return -1;
    }

    int new_flags = old_flags | O_NONBLOCK;
    int error = ::fcntl(fd, F_SETFL, new_flags);

    DebugAssertWithInfo(error >= 0, "maybe system error");

    if(error < 0)
    {
        return -1;
    }

    return 0;
}

int SetFdReUseAddr(int fd)
{
    int opt = 1;
    int err = 0;
    err = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    DebugAssertWithInfo(err == 0, "setsockopt() call error!");
    if(err < 0) {
        GAME_EXT1_LOG_ERROR("[SetFdReUseAddr] setsockopt() call error! fd=%d, errno=%d", fd, errno);
    }
    return err;
}

}// namespace end