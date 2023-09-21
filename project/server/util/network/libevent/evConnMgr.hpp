#pragma once
#include "util/network/ConnMgr.hpp"
#include "util/network/libevent/evConnection.hpp"
#include <map>
#include <event2/event.h>

namespace util::network::ev
{

//TODO 当前evConnMgr是不完备的，它的职责还没有明确，因此还没有完成编写
/**
 * @brief 连接管理器，经常在多个线程中被访问到，需要注意接口的线程安全
 */
class evConnMgr:
    public ConnMgr
{
public:
    static const std::unique_ptr<evConnMgr>& GetInstance();
    virtual ~evConnMgr();

    /**
     * @brief 创建一个 evConnection 对象，并返回指针
     * 
     * @param thread 这个连接IO活跃所在的线程
     * @param newfd 这个连接对应的连接
     * @param peer_ip 对端的ip、port
     * @param local_ip 本地的ip、port
     * @return ConnectionSPtr 
     */
    virtual ConnectionSPtr CreateConn(IOThread* thread, int newfd, Address peer_ip, Address local_ip) BBTATTR_FUNC_RetVal;
    /*XXX 通过socketfd获取连接，寻求更好的方式，暴露出socket fd可能是不好的，可以加入句柄/id */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd); 

    /* 设置IO Thread */
    void SetIOThread(IOThread*);
private:
    evConnMgr();

    void InitCfg();
    void InitEvent();

    void OnConnInit(evConnectionSPtr conn);
    void OnConnDestory(evConnectionSPtr conn);
private:
    /* 心跳事件控制 */
    void InitHeartBeatEvent();
    void DestoryHeartBeatEvent();
private:
    /** 心跳检测：遍历并检测所有连接事件，对超时的连接事件进行通知
     * 在acceptor线程里面 event_base 里面注册这个事件。定时调用，以
     * 实现定时检测心跳。在acceptor线程的原因是它的负载低
     */
    void OnHeartBeatOnce();
private:
    std::map<evutil_socket_t, ConnectionSPtr> m_conn_map;
    std::mutex  m_conn_map_mutex;
    IOThread*   m_thread{nullptr};
    /* 心跳超时的时间间隔 */
    bbt::timer::clock::ms m_heartbeat_timeout_duration;
    /* 心跳检查的时间间隔 */
    int         m_check_heartbeat_interval_ms;
    event*      m_heartbeat_check_event;
};

}// namespace end