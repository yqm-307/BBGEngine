#pragma once
#include "util/network/Connection.hpp"

namespace util::network
{

class ConnMgr
{
public:
    ConnMgr();
    virtual ~ConnMgr() = 0;

    /* 创建Conn的接口 */
    // virtual ConnectionSPtr CreateConn(int newfd, void* args) = 0;
    /* 通过socketfd获取连接 */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd) = 0; 
};

}// namespace end