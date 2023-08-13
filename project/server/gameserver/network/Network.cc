#include "gameserver/network/Network.hpp"

namespace server::network
{

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
	tv.tv_sec = 2;
    TestInfo* info = new TestInfo();
    info->info = info_str;
    event* test_timer = event_new(base, -1, EV_TIMEOUT | EV_PERSIST, Test_Timer, info);
    event_add(test_timer, &tv);
}

#endif

Network::Network(const std::string& ip, short port)
    :m_listen_ip(ip),
    m_listen_port(port),
    m_io_thread_num(3)
{
    m_thread_latch = new bbt::thread::lock::CountDownLatch(4);
    for(int i = 0; i < m_io_thread_num; ++i)
    {
        m_io_threads.push_back(new IOThread());
        if(i == 0)
            m_io_threads[i]->SetWorkFunc([=](){ 
                this->AcceptWork(i); });
        else
            m_io_threads[i]->SetWorkFunc([=](){
                this->IOWork(i); });
    }

    GAME_BASE_LOG_INFO("Network init success!");
}

Network::~Network() {}

void Network::SyncStart()
{
    for(int i = 0; i < m_io_thread_num; ++i)
        m_io_threads[i]->Start();
    WaitForOtherIOThreadStart();
}

void Network::IOWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("IO thread start!");
#ifdef Debug
    Test_AddEvent(ev_base, "io work test timer!");
#endif
    event_base_dispatch(ev_base);
}

void Network::AcceptWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("Accept thread start!");
#ifdef Debug
    Test_AddEvent(ev_base, "accept test timer!");
#endif
    // ev_base
    event_base_dispatch(ev_base);
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