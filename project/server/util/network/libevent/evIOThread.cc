#include "util/network/libevent/evIOThread.hpp"

namespace util::network::ev
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
        GAME_BASE_LOG_INFO("IOThread Start! tid = %d", tid);
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
    BBTATTR_COMM_Unused int error = event_base_loopbreak(m_ev_base);
    
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

EventId evIOThread::RegisterEvent(std::shared_ptr<evEvent> evptr)
{
    int err = 0;
    DebugAssert(evptr != nullptr);
    err = evptr->Register(m_ev_base);
    if(err < 0) {
        return -1;
    }
    // 添加到 event map 中
    auto [it, isok] = m_event_map.insert(std::make_pair(evptr->GetEventID(), evptr));
    if(!isok) {
        /* 这里即使错误仍然是返回错误，不需要处理错误情况，但是可能需要打个日志 */
        err = evptr->UnRegister();
        return -1;
    }

    return 0;
}

EventId evIOThread::RegisterEventSafe(std::shared_ptr<evEvent> evptr)
{
    int err = 0;
    DebugAssert(evptr != nullptr);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        err = evptr->Register(m_ev_base);
        if(err < 0) {
            return -1;
        }

        auto [it, isok] = m_event_map.insert(std::make_pair(evptr->GetEventID(), evptr));
        if(!isok) {
            err = evptr->UnRegister();
            return -1;
        }
    }

    return 0;
}


int evIOThread::UnRegisterEvent(EventId eventid)
{
    auto it = m_event_map.find(eventid);
    if(it == m_event_map.end())
    {
        return -1;
    }
    
    m_event_map.erase(it);

    auto isok = (it->second)->UnRegister();

    return isok;
}

std::pair<errcode::ErrOpt, EventId>
evIOThread::RegisterEventSafe(const EventCallback& cb, evutil_socket_t fd, short events, int target_interval_ms)
{
    auto event = std::make_shared<evEvent>(cb, fd, events, target_interval_ms);
    int err = 0;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        err = event->Register(m_ev_base);
        if (err < 0) {
            return {
                errcode::ErrCode("event register failed!", 
                                errcode::ErrType::NetWorkErr, 
                                errcode::errenum::MODULE_NETWORK::Event_Register_Failed), 
                -1};
        }

        auto [it, isok] = m_event_map.insert(std::make_pair(event->GetEventID(), event));
        if (!isok) {
            err = event->UnRegister();
            return {
                errcode::ErrCode("event register repeat!",
                                errcode::ErrType::NetWorkErr,
                                errcode::errenum::MODULE_NETWORK::Event_Register_Failed),
                -1};
        }
    }

    return {std::nullopt, event->GetEventID()};
}

}// namespace end