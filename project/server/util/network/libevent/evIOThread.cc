#include "util/network/libevent/evIOThread.hpp"

namespace game::util::network::ev
{

evIOThread::evIOThread(const IOThread::WorkCallback& work_cb)
    :IOThread(work_cb)
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


}// namespace end