#include "gameserver/network/Network.hpp"

namespace server::network
{

Network::Network(const std::string& ip, short port)
    :m_listen_ip(ip),
    m_listen_port(port)
{
    int count = 4;
    m_thread_latch = new bbt::thread::lock::CountDownLatch(4);
    for(int i = 0; i < count; ++i)
    {
        m_io_threads.push_back(new IOThread(OnCreateEventBase()));
        if(i == 0)
            m_io_threads[i]->SetWorkTask([=](){ 
                this->AcceptWork(i); });
        else
            m_io_threads[i]->SetWorkTask([=](){
                this->IOWork(i); });
    }
    for(int i = 0; i < count - 1; ++i)
        m_io_threads[i]->Start();
    WaitForOtherIOThreadStart();
    GAME_BASE_LOG_INFO("Network init success!");
}

Network::~Network() {}

bool Network::SyncStart()
{
    
}

void Network::IOWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("IO thread start!");
    event_base_dispatch(ev_base);
}

void Network::AcceptWork(int index)
{
    WaitForOtherIOThreadStart();
    auto ev_base = m_ev_bases[index];
    GAME_BASE_LOG_INFO("Accept thread start!");
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