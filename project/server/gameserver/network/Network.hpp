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
    void SyncStart();
private:
    /* io 工作线程 */
    void IOWork(int index);
    /* accept 线程 */
    void AcceptWork(int index);
    /* 等待所有IO线程，启动完成 */
    void WaitForOtherIOThreadStart();
    event_base* OnCreateEventBase();
    void OnDestoryEventBase(event_base* base);
private:
    std::string     m_listen_ip;
    int             m_listen_port;
    size_t          m_io_thread_num;
    std::vector<IOThread*> m_io_threads;    // 下标0是acceptor线程
    std::vector<event_base*> m_ev_bases;
    bbt::thread::lock::CountDownLatch* m_thread_latch;
};

}