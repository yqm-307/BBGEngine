#pragma once
#include "util/network/libevent/Define.hpp"

namespace game::util::network::ev
{


// XXX 实现比较粗糙，可能不够RAII，如果可以使用shared_ptr管理更好
class evEvent
{
public:
    evEvent() = delete;
    explicit evEvent(const EventCallback& cb, evutil_socket_t fd, short events);
    ~evEvent();

    /* 触发event事件 */ //XXX 是否需要 void*，因为这是C的做法，C++使用函数对象即可
    void OnEvent(evutil_socket_t fd, short events);

    /* 将此事件注册到一个 event_base 中，此函数是不可重入的 */
    int RegisterInEvBase(event_base* base, int target_interval_ms) BBTATTR_FUNC_RetVal;
    int UnRegister() BBTATTR_FUNC_RetVal;
private:
    EventCallback   m_callback{nullptr};
    short   m_events{-1};
    int     m_fd{-1};
    event*  m_event{nullptr};
};

}// namespace end