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
    typedef std::function<void()> OnEventCallback;

    evIOThread(event_base* base);
    evIOThread(const IOThread::WorkCallback& work_cb);
    virtual ~evIOThread();

    virtual void Start() override; 

    /**
     * @brief Set the On Destory object
     * 
     */
    void SetOnDestory(const OnEventCallback& cb);
    void Init(const WorkCallback& cb);
protected:
    event_base*     m_ev_base{nullptr};
    OnEventCallback m_ondestory_cb;
};

};

}// namespace end