#include "util/network/libevent/evIOThread.hpp"

namespace game::util::network::ev
{

void OnRecvFunc(evutil_socket_t fd, short event, void* args)
{

}

evIOThread::evIOThread()
{
    Init();
}

evIOThread::~evIOThread()
{
    /* 等待线程停止 */
    if(m_status == IOThreadRunStatus::Running)
        Stop();        
    /* 资源回收 */
    Destory();
}

void evIOThread::Init()
{

    SetOnThreadBegin_Hook([](IOThreadID tid){
        GAME_BASE_LOG_INFO("IOThread Start Success! tid = %d", tid);
    });
    
    SetOnThreadEnd_Hook([](IOThreadID tid){
        GAME_BASE_LOG_INFO("IOThread Exit! tid = %d", tid);
    });
}

void evIOThread::Destory()
{
}


void evIOThread::evWorkFunc()
{
    m_io_work_func();
}

void evIOThread::Stop()
{
    DebugAssert(m_ev_base != nullptr);
    int error = event_base_loopbreak(m_ev_base);
    
    /* 阻塞式的等待 */
    SyncWaitThreadExit();
    DebugAssert(error == 0);
}

void evIOThread::SetWorkFunc(const IOWorkFunc& cb)
{
    m_io_work_func = cb;
    AssertWithInfo(m_io_work_func != nullptr, "io work need be set!");
    SetWorkTask([=](){ evWorkFunc(); });
}

void evIOThread::SetEventBase(event_base* ev_base)
{
    DebugAssertWithInfo(ev_base != nullptr, "this is a fatal bug!");
    m_ev_base = ev_base;
}

int evIOThread::Register_OnRecv(evutil_socket_t sockfd, const OnRecvCallback& onrecv_cb)
{
    // todo 实现细节，注册事件
}

int evIOThread::UnRegister_OnRecv(event* ev)
{
    // todo 实现细节，注销接收事件
}

int evIOThread::Register_HeartBeat(evutil_socket_t sockfd, const OnTimeOutCallback& time_out_cb)
{
    // todo 实现细节，注册超时事件
}

int evIOThread::UnRegister_HeartBeat(event* ev)
{
    // todo 实现细节，注销超时事件
}

}// namespace end