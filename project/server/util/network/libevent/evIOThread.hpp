#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"
#include <bbt/timer/Clock.hpp>
#include <bbt/buffer/Buffer.hpp>

namespace game::util::network
{

namespace ev
{

class evIOThread:
    public game::util::network::IOThread
{
public:
    typedef std::function<void()> IOWorkFunc;
    typedef std::function<void(const bbt::buffer::Buffer&, const util::errcode::ErrCode&)> OnRecvCallback;
    typedef std::function<void(const util::errcode::ErrCode&)> OnTimeOutCallback;

    evIOThread();
    virtual ~evIOThread();

    /* 非阻塞的停止thread（实现有问题，需要阻塞） */
    virtual void Stop() override;

    /* 线程启动前需要初始化（必选） */
    void SetWorkFunc(const IOWorkFunc& cb);

    /* 线程启动前需要初始化（必选） */
    void SetEventBase(event_base* ev_base);

    virtual int Register_OnRecv(evutil_socket_t sockfd, const OnRecvCallback& onrecv_cb);
    virtual int UnRegister_OnRecv(event* ev);
    virtual int Register_HeartBeat(evutil_socket_t sockfd, const OnTimeOutCallback& time_out_cb);
    virtual int UnRegister_HeartBeat(event* ev);

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
};

};

}// namespace end