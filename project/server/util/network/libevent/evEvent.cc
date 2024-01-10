#include "util/network/libevent/evEvent.hpp"

namespace util::network::ev
{

//------------------------------ 局部函数实现 --------------------------------//

/**
 * 作为转发函数，对于 libevent 来说，将 C style函数转发到 C++ style
*/
void OnEvent(evutil_socket_t fd, short events, void* args)
{
    auto event = reinterpret_cast<evEvent*>(args);
    event->OnEvent(fd, events);
}

//------------------------------ 类实现 --------------------------------//
evEvent::evEvent(const EventCallback& cb, evutil_socket_t fd, short events, int target_interval_ms)
    :m_callback(cb),
    m_fd(fd),
    m_events(events)
{
    DebugAssert(m_callback != nullptr);
    DebugAssert(m_events >= 0);

    evutil_timerclear(&m_timeout);
    if(target_interval_ms > 0){
        m_timeout.tv_sec  = target_interval_ms / 1000;
        m_timeout.tv_usec = (target_interval_ms % 1000) * 1000;
    }
    m_event_id = GenerateID();
}


evEvent::~evEvent()
{
    // XXX 也许这里不需要释放
    int err = UnRegister();
    if (err >= 0) {
        GAME_BASE_LOG_WARN("[evEvent::~evEvent] unresgiste failed");
    }

    event_free(m_event);
    m_event = nullptr;
    m_event_id = -1;
}

void evEvent::OnEvent(evutil_socket_t fd, short events)
{
    DebugAssert(m_callback != nullptr);
    m_callback(fd, events, nullptr);
}

int evEvent::Register(event_base* base)
{
    int err = 0;

    // 不允许重复注册，因为可能导致事件覆盖
    DebugAssert(m_event == nullptr);

    if(m_event != nullptr) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] repeat registration is not allowed! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    m_event = event_new(base, m_fd, m_events, ev::OnEvent, this);
    DebugAssert(m_event != nullptr);

    if(m_event == nullptr) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] event_new() error! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    err = event_add(m_event, &m_timeout);
    DebugAssert(err == 0);

    if(err != 0) {
        GAME_BASE_LOG_ERROR("[evEvent::RegisterInEvBase] event_add() error! fd=%d\tevents=%d", m_fd, m_events);
        return -1;
    }

    return 0;
}

int evEvent::UnRegister()
{
    // XXX 也许这里会反复的释放
    int err = 0;
    err = event_del(m_event);
    DebugAssert(err == 0);

    if(err != 0) {
        return -1;
    }

    return 0;
}

EventId evEvent::GenerateID()
{
    static std::atomic_int32_t global_id = 1;
    return global_id++;
}

EventId evEvent::GetEventID() const
{
    return m_event_id;
}

}// namespace end