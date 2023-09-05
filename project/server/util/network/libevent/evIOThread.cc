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

int evIOThread::Register_OnRecv(evutil_socket_t sockfd, evArgs* args)
{
    // XXX 选择什么形式实现参数生命期的管理，1、静态的，event对象保存在evConnection中。2、动态的，需要进行详细的生命期管理
    // 目前是静态的，保存在evConnetion，由于evConnection释放时UnRegister，倒也不会有啥问题吧
    DebugAssert(args != nullptr);
    event* event = event_new(m_ev_base, sockfd, EV_READ | EV_PERSIST, game::util::network::OnRecvCallback, args);
    return (event != nullptr);
}

int evIOThread::UnRegister_OnRecv(event* ev)
{
    int err = event_del(ev);
    DebugAssert(err >= 0);
    return err;
}

int evIOThread::RegisterEvent(evutil_socket_t fd, short events, const EventCallback& callback, void* args)
{
    //TODO 没有实现完毕
    __attribute_maybe_unused__ event* new_event = nullptr;
    DebugAssert(fd >= 0 && events > 0 && callback != nullptr);
    return -1;
}

int evIOThread::UnRegisterEvent(int eventid)
{
    return -1;
}

}// namespace end