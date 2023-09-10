#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"
#include "util/network/libevent/evIOThread.hpp"
#include <bbt/timer/Clock.hpp>
#include <bbt/buffer/Buffer.hpp>
#include <map>

namespace game::util::network
{

namespace ev
{

struct Event;


class evIOThread:
    public game::util::network::IOThread
{
public:
    typedef std::function<void()> IOWorkFunc;
    typedef std::function<void(const bbt::buffer::Buffer&, const util::errcode::ErrCode&)> OnRecvCallback;
    typedef std::function<void(const util::errcode::ErrCode&)> OnTimeOutCallback;
    typedef std::function<void(evutil_socket_t, short, void*)> EventCallback;

    evIOThread();
    virtual ~evIOThread();

    /* 非阻塞的停止thread（实现有问题，需要阻塞） */
    virtual void Stop() override;

    /* 线程启动前需要初始化（必选） */
    void SetWorkFunc(const IOWorkFunc& cb);

    /* 线程启动前需要初始化（必选） */
    void SetEventBase(event_base* ev_base);

    /**
     * @brief 在当前线程中注册一个事件
     * 
     * @param event_ptr 
     * @return EventId 如果成功返回事件id，失败返回-1
     */
    EventId RegisterEvent(std::shared_ptr<evEvent> event_ptr) BBTATTR_FUNC_RetVal;

    /**
     * @brief 从此线程监听的事件中取消一个事件
     * 
     * @param eventid 事件id
     * @return int 成功返回0，失败返回-1
     */
    int UnRegisterEvent(EventId eventid) BBTATTR_FUNC_RetVal;

    // virtual int Register_OnRecv(evutil_socket_t sockfd, const OnSendCallback& onsend_cb);
private:
    /* 初始化线程内部资源 */
    void Init();
    /* 回收线程内部资源 */
    void Destory();
    
    void evWorkFunc();
protected:
    event_base*     m_ev_base{nullptr};
    IOWorkFunc      m_io_work_func{nullptr};
    // TODO 统一的event事件管理，使用智能指针管理
    std::map<EventId, std::shared_ptr<evEvent>>   m_event_map;
};

struct Event
{
    event* m_event;
    evIOThread::EventCallback m_callback;
    void* args;
};

}// namespace end
}// namespace end