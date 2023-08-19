#pragma once
#include "util/network/ConnMgr.hpp"
#include "util/network/libevent/evConnection.hpp"
#include <map>

namespace game::util::network::ev
{

class evConnMgr:
    public ConnMgr
{
public:
    static evConnMgr* GetInstance();

    virtual ConnectionSPtr CreateConn(int newfd, void* args);
    /* 通过socketfd获取连接 */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd); 
private:
    evConnMgr();
    virtual ~evConnMgr();
private:
    std::map<evutil_socket_t, ConnectionSPtr> m_conn_map;
};

}// namespace end