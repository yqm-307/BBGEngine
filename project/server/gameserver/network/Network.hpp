#pragma once
#include <event2/event.h>
#include <string>
#include <bbt/thread/Lock.hpp>
#include "util/network/libevent/evIOThread.hpp"


namespace server::network
{

class Network
{
public:
    typedef game::util::network::ev::evIOThread IOThread;

    Network(const std::string& ip, short port);
    ~Network();
    /* 同步的启动所有线程，等所有IO线程全部启动返回 */
    void SyncStart();
    /* 同步的停止所有IO线程，等所有IO线程全部停止后返回 */
    void SyncStop();
private:
    /* 对象内部数据申请 */
    void Init();
    /* 对象内部数据释放 */
    void Destory();

    /* io 工作线程 */
    void IOWork(int index);
    /* accept 线程 */
    void AcceptWork(int index);
    /* 等待所有IO线程，启动完成 */
    void WaitForOtherIOThreadStart();
    /* libevent event_base 构造 */
    event_base* OnCreateEventBase();
    /* libevent event_base 释放 */
    void OnDestoryEventBase(event_base* base);
private:
    std::string     m_listen_ip;
    int             m_listen_port;
    size_t          m_io_thread_num;
    std::vector<IOThread*> m_io_threads;    // 下标0是acceptor线程
    std::vector<event_base*> m_ev_bases;    // 每个线程一个 event_base
    bbt::thread::lock::CountDownLatch* m_thread_latch;
};

}