#pragma once
#include "util/network/ConnMgr.hpp"
#include "util/network/libevent/evConnection.hpp"
#include <map>

namespace game::util::network::ev
{

//TODO 当前evConnMgr是不完备的，它的职责还没有明确，因此还没有完成编写
/**
 * @brief 连接管理器，经常在多个线程中被访问到，需要注意接口的线程安全
 */
class evConnMgr:
    public ConnMgr
{
public:
    static evConnMgr* GetInstance();

    /**
     * @brief 创建一个 evConnection 对象，并返回指针
     * 
     * @param thread 这个连接IO活跃所在的线程
     * @param newfd 这个连接对应的连接
     * @param peer_ip 对端的ip、port
     * @param local_ip 本地的ip、port
     * @return ConnectionSPtr 
     */
    virtual ConnectionSPtr CreateConn(IOThread* thread, int newfd, Address peer_ip, Address local_ip);
    /*XXX 通过socketfd获取连接，寻求更好的方式，暴露出socket fd可能是不好的，可以加入句柄/id */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd); 
private:
    evConnMgr();
    virtual ~evConnMgr();

    void OnConnInit(evConnectionSPtr conn);
    void OnConnDestory(evConnectionSPtr conn);
private:
    std::map<evutil_socket_t, ConnectionSPtr> m_conn_map;
    std::mutex  m_conn_map_mutex;
};

}// namespace end