#pragma once
#include "util/network/libevent/Define.hpp"

namespace game::util::network::ev
{

void _EventCallbackTransform(evutil_socket_t fd, short events, void* args);

// XXX 实现比较粗糙，可能不够RAII，如果可以使用shared_ptr管理更好
// XXX 是否需要可复用或者event池，如果足够频繁的变动是可以减少内存消耗和cpu消耗
class evEvent
{
    friend void _EventCallbackTransform(evutil_socket_t fd, short events, void* args);
public:
    evEvent() = delete;
    explicit evEvent(const EventCallback& cb, evutil_socket_t fd, short events);
    ~evEvent();


    /* 将此事件注册到一个 event_base 中，此函数是不可重入的 */
    int RegisterInEvBase(event_base* base, int target_interval_ms) BBTATTR_FUNC_RetVal;
    int UnRegister() BBTATTR_FUNC_RetVal;
private:
    /* 触发event事件 */
    void OnEvent(evutil_socket_t fd, short events);
    void OnInit();
    void OnDestory();
    /* XXX 获取一个不太可能重复的id，但是仍然有可能会重复 */
    EventId GenerateID();
private:
    EventId m_event_id;
    EventCallback   m_callback{nullptr};
    short   m_events{-1};
    int     m_fd{-1};
    event*  m_event{nullptr};
};

}// namespace end