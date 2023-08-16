#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"
#include "bbt/timer/Clock.hpp"

namespace game::util::network
{

namespace ev
{

class evIOThread:
    public game::util::network::IOThread
{
public:
    typedef std::function<void()> IOWorkFunc;

    evIOThread();
    virtual ~evIOThread();

    /* 非阻塞的停止thread（实现有问题，需要阻塞） */
    virtual void Stop() override;

    /* 线程启动前需要初始化（必选） */
    void SetWorkFunc(const IOWorkFunc& cb);

    /* 线程启动前需要初始化（必选） */
    void SetEventBase(event_base* ev_base);
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