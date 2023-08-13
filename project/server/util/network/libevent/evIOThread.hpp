#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"

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

    virtual void Start() override;
    virtual void Stop() override;

    void SetWorkFunc(const IOWorkFunc& cb);

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