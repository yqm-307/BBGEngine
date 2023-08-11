#include "util/network/libevent/evIOThread.hpp"

namespace game::util::network::ev
{

evIOThread::evIOThread(event_base* base)
    :m_ev_base(base)
{
    AssertWithInfo(m_ev_base != nullptr, "evIOThread init fatal!");
}


evIOThread::evIOThread(const IOThread::WorkCallback& work_cb)
{}

evIOThread::~evIOThread()
{
    if(m_ondestory_cb != nullptr)
        m_ondestory_cb();

    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
}

void evIOThread::Start()
{
    m_thread = new std::thread(m_work_callback);
}

void evIOThread::SetOnDestory(const OnEventCallback& cb)
{
    m_ondestory_cb = cb;
}

void evIOThread::Init(const WorkCallback& cb)
{
    SetWorkTask(cb);
}


}// namespace end