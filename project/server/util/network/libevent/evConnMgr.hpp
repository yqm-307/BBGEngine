#pragma once
#include "util/network/ConnMgr.hpp"
#include "util/network/libevent/evConnection.hpp"
#include <map>

namespace game::util::network::ev
{

//TODO 当前evConnMgr是不完备的，它的职责还没有明确，因此还没有完成编写
class evConnMgr:
    public ConnMgr
{
public:
    static evConnMgr* GetInstance();

    virtual ConnectionSPtr CreateConn(int newfd, void* args);
    /*XXX 通过socketfd获取连接，寻求更好的方式，暴露出socket fd可能是不好的，可以加入句柄/id */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd); 
private:
    evConnMgr();
    virtual ~evConnMgr();
private:
    std::map<evutil_socket_t, ConnectionSPtr> m_conn_map;
};

}// namespace end