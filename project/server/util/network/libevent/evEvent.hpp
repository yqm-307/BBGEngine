#pragma once
#include "util/network/libevent/Define.hpp"

namespace util::network::ev
{

void _EventCallbackTransform(evutil_socket_t fd, short events, void* args);



/** //XXX 
 * 1、是否需要可复用或者event池，如果足够频繁的变动是可以减少内存消耗和cpu消耗。
 * 2、是否需要将事件系统和libevent的事件系统结合起来做一个统一的事件通知机制。
*/
class evEvent
{
    friend void _EventCallbackTransform(evutil_socket_t fd, short events, void* args);
    friend class evIOThread;
public:
    evEvent() = delete;
    
    explicit evEvent(const EventCallback& cb, evutil_socket_t fd, short events, int target_interval_ms) BBTATTR_FUNC_Ctor_Hidden;
    ~evEvent();


    /**
     * @brief 创建evEvent对象，系统中很多这种接口设计，本意是想让这个类不会被创建裸指针
     * 
     * @param cb    监听事件，当监听的事件发生时触发。
     * @param fd    文件描述符，当监听的事件和文件描述符相关
     * @param events    监听的事件类型，当前只包含libevent的事件类型
     * @return std::shared_ptr<evEvent> 创建后的智能指针
     */
    static std::shared_ptr<evEvent> Create(const EventCallback& cb, evutil_socket_t fd, short events, int target_interval_ms = -1);

    /* 获取 event id，id应该大于0。如果id小于0，有可能是值溢出或者错误 */
    EventId GetEventID() const;
private:

    /* 将此事件注册到一个 event_base 中，此函数是不可重入的 */
    int RegisterInEvBase(event_base* base) BBTATTR_FUNC_RetVal;
    /* 注销此事件，成功返回0，失败返回-1 */
    int UnRegister() BBTATTR_FUNC_RetVal;

    /* 触发event事件 */
    void OnEvent(evutil_socket_t fd, short events);
    void OnInit(int timeout);
    void OnDestory();
    /* XXX 获取一个不太可能重复的id，但是仍然有可能会重复 */
    EventId GenerateID();
private:
    EventId m_event_id;
    EventCallback   m_callback{nullptr};
    short   m_events{-1};
    int     m_fd{-1};
    timeval m_timeout;
    event*  m_event{nullptr};
};

}// namespace end