#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"
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
     * @brief 在此线程中注册一个事件
     * 
     * @param fd 如果不是fd相关的事件，可以设置为0
     * @param events 关注事件的类型，参考宏EV_TIMEOUT
     * @param callback 事件发生时调用的回调函数
     * @param args callback的参数（大部分情况使用函数对象即可）
     * @return int 成功返回大于等于0的eventid，失败返回-1
     */
    [[nodiscard("需要做错误处理")]]int RegisterEvent(evutil_socket_t fd, short events, const EventCallback& callback, void* args);

    /**
     * @brief 从此线程监听的事件中取消一个事件
     * 
     * @param eventid 事件id
     * @return int 成功返回0，失败返回-1
     */
    [[nodiscard("需要做错误处理")]]int UnRegisterEvent(int eventid);


    // TODO 需要用统一的RegisterEvent接口替换掉下面的接口
    int Register_OnRecv(evutil_socket_t sockfd, evArgs* onrecv_cb);
    int UnRegister_OnRecv(event* ev);

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
    std::map<int, Event*>   m_event_map;
};

struct Event
{
    event* m_event;
    evIOThread::EventCallback m_callback;
    void* args;
};

}// namespace end
}// namespace end