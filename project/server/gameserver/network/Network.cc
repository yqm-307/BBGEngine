#include "gameserver/network/Network.hpp"

namespace server::network
{
// #define Debug
#ifdef Debug

struct TestInfo
{
    std::string info;
};


void Test_Timer(evutil_socket_t , short, void* arg)
{
    auto info = (TestInfo*)arg;
    GAME_BASE_LOG_INFO("Test Timer Info: %s", info->info.c_str());
}

void Test_AddEvent(event_base* base, const std::string& info_str)
{
    timeval tv;
    evutil_timerclear(&tv);
	tv.tv_sec = 2;
    TestInfo* info = new TestInfo();
    info->info = info_str;
    event* test_timer = event_new(base, -1, EV_PERSIST, Test_Timer, info);
    event_add(test_timer, &tv);
}

#endif

Network::Network(const std::string& ip, short port)
    :m_listen_ip(ip),
    m_listen_port(port),
    m_io_thread_num(3)
{
    Init();
    GAME_BASE_LOG_INFO("Network init success!");
}

Network::~Network() 
{
    Destory();
    GAME_BASE_LOG_INFO("Network exit success!");
}

void Network::Init()
{
    /* 主线程 + accept线程 + io线程 */
    m_thread_latch = new bbt::thread::lock::CountDownLatch(m_io_thread_num + 1);

    /* IO线程和libevent初始化 */
    for(int i = 0; i < m_io_thread_num; ++i)
    {
        /* 初始化io线程的回调 */
        m_io_threads.push_back(new IOThread());
        if(i == 0)
            m_io_threads[i]->SetWorkFunc([=](){ AcceptWork(i); });
        else        
            m_io_threads[i]->SetWorkFunc([=](){ IOWork(i); });

        /* 初始化libevent */
        auto ev_base = OnCreateEventBase();
        Assert(ev_base != nullptr);
        m_io_threads[i]->SetEventBase(ev_base);
    }

    
}

void Network::Destory()
{
    delete m_thread_latch;
    m_thread_latch = nullptr;

    for (int i = 0; i < m_io_thread_num; i++)
    {
        DebugAssert( !m_io_threads[i]->IsRunning() );
        if(m_io_threads[i]->IsRunning())
           GAME_BASE_LOG_WARN("io thread status abnormal!");

        delete m_io_threads[i];
        OnDestoryEventBase(m_ev_bases[i]);
        m_ev_bases[i] = nullptr;
    }
}

void Network::SyncStart()
{
    for(int i = 0; i < m_io_thread_num; ++i)
    {
        m_io_threads[i]->Start();
    }
    WaitForOtherIOThreadStart();
}

void Network::SyncStop()
{
    for(int i = 0; i < m_io_thread_num; ++i)
        m_io_threads[i]->Stop();
}

void Network::IOWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("IO thread start!");
#ifdef Debug
    Test_AddEvent(ev_base, "io work test timer!");
#endif
    int error = event_base_dispatch(ev_base);
    AssertWithInfo(error == 0, "libevent error!");
}

void Network::AcceptWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("Accept thread start!");
#ifdef Debug
    Test_AddEvent(ev_base, "accept test timer!");
#endif
    int error = event_base_dispatch(ev_base);
    AssertWithInfo(error == 0, "libevent error!");
}

void Network::WaitForOtherIOThreadStart()
{
    m_thread_latch->down();
    m_thread_latch->wait();
}

event_base* Network::OnCreateEventBase()
{
    auto base = event_base_new();
    m_ev_bases.push_back(base);
    return base;
}

void Network::OnDestoryEventBase(event_base* base)
{
    auto it = std::find_if(m_ev_bases.begin(), m_ev_bases.end(), 
        [=](event_base* val){
            return val == base;
        }
    );
    DebugAssertWithInfo(it != m_ev_bases.end(), "event base is unexpected value!");
    m_ev_bases.erase(it);
}

}// namespace end