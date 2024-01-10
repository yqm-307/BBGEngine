#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"
#include <bbt/timer/Clock.hpp>
#include <bbt/buffer/Buffer.hpp>
#include <bbt/poolutil/IDPool.hpp>
#include <map>

namespace util::network
{

namespace ev
{


class evIOThread:
    public util::network::IOThread
{
public:
    typedef std::function<void()> IOWorkFunc;
    typedef std::function<void(const bbt::buffer::Buffer&, const util::errcode::ErrCode&)> OnRecvCallback;
    typedef std::function<void(const util::errcode::ErrCode&)> OnTimeOutCallback;
    typedef std::function<void(evutil_socket_t, short, void*)> EventCallback;

    evIOThread();
    virtual ~evIOThread();

    /* 阻塞的停止thread */
    virtual void Stop() override;

    /* 线程启动前需要初始化（必选） */
    void SetWorkFunc(const IOWorkFunc& cb);

    /* 线程启动前需要初始化（必选） */
    void SetEventBase(event_base* ev_base);

    /* 注册一个libevent事件 */
    std::pair<errcode::ErrOpt, EventId>
    RegisterEventSafe(const EventCallback& cb, evutil_socket_t fd, short events, int target_interval_ms) BBTATTR_FUNC_RetVal;

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
    std::map<EventId, std::shared_ptr<evEvent>>   m_event_map;
    std::mutex      m_mutex;
};


}// namespace end
}// namespace end