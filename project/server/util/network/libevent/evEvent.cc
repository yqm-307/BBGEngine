#include "util/network/libevent/evEvent.hpp"

namespace game::util::network::ev
{

//------------------------------ 局部函数实现 --------------------------------//

/**
 * 作为转发函数，对于 libevent 来说，将 C style函数转发到 C++ style
*/
void _EventCallbackTransform(evutil_socket_t fd, short events, void* args)
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
    OnInit(target_interval_ms);
}


evEvent::~evEvent()
{
    OnDestory();
}

void evEvent::OnEvent(evutil_socket_t fd, short events)
{
    DebugAssert(m_callback != nullptr);
    m_callback(fd, events, nullptr);
}

int evEvent::RegisterInEvBase(event_base* base)
{
    int err = 0;

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
    int err = 0;

    err = event_del(m_event);
    DebugAssert(err == 0);

    if(err != 0) {
        return -1;
    }

    return 0;
}

void evEvent::OnInit(int timeout)
{
    evutil_timerclear(&m_timeout);
    if(timeout > 0){
        m_timeout.tv_sec  = timeout / 1000;
        m_timeout.tv_usec = (timeout % 1000) * 1000;
    }
    m_event_id = GenerateID();
}

void evEvent::OnDestory()
{
    // TODO 也许需要注销事件？应该需要，因为对象释放就说明事件监听者已经死亡了
    // UnRegister();
    event_free(m_event);
    m_event = nullptr;
    m_event_id = -1;
}

EventId evEvent::GenerateID()
{
    /**
     * 这样写的好处是，避免在类内声明静态变量。
     * 
     * 类内静态变量的初始化规则比较不好理解，需要初始化在源文件，
     * 涉及到一些链接相关的知识需要理解。
    */
    static std::atomic_int32_t global_id = 1;
    return global_id++;
}

EventId evEvent::GetEventID() const
{
    return m_event_id;
}

std::shared_ptr<evEvent> evEvent::Create(const EventCallback& cb, evutil_socket_t fd, short events, int timeout_interval_ms)
{
    return std::make_shared<evEvent>(cb, fd, events, timeout_interval_ms);
}

}// namespace end