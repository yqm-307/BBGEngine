#include "util/network/libevent/evEvent.hpp"

namespace game::util::network::ev
{

//------------------------------ 局部函数实现 --------------------------------//
void _EventCallbackTransform(evutil_socket_t fd, short events, void* args)
{
    auto event = reinterpret_cast<evEvent*>(args);
}

//------------------------------ 类实现 --------------------------------//
explicit evEvent::evEvent(const EventCallback& cb, evutil_socket_t fd, short events)
    :m_callback(cb),
    m_fd(fd),
    m_events(events)
{
    DebugAssert(m_callback != nullptr);
    DebugAssert(m_events >= 0);
}


evEvent::~evEvent()
{

}

void evEvent::OnEvent(evutil_socket_t fd, short events)
{
    m_callback(fd, events, nullptr);
}

int evEvent::RegisterInEvBase(event_base* base, int target_interval_ms)
{
    int err = 0;
    timeval tv;
    evutil_timerclear(&tv);

    // 不允许重复注册，因为可能导致事件覆盖
    DebugAssert(m_event == nullptr);

    if(m_event != nullptr) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] repeat registration is not allowed! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    m_event = event_new(base, m_fd, m_events, _EventCallbackTransform, this);
    DebugAssert(m_event != nullptr);

    if(m_event == nullptr) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] event_new() error! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    tv.tv_usec = (target_interval_ms * 1000);
    err = event_add(m_event, &tv);
    DebugAssert(err == 0);

    if(err != 0) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] event_add() error! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    return 0;
}

int evEvent::UnRegister()
{
    int err = 0;

    err = event_del(m_event);
    DebugAssert(err == 0);

    if(err != 0) {
        GAME_BASE_LOG_ERROR("[evEvent::UnRegister] event_del() error!");
        return -1;
    }

    return 0;
}

}// namespace end