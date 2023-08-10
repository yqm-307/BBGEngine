#pragma once
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/IOThread.hpp"

namespace game::util::network
{

namespace ev
{

class evIOThread:
    public IOThread
{
public:
    typedef std::function<void()> OnEventCallback;

    evIOThread(const IOThread::WorkCallback& work_cb);
    virtual ~evIOThread();

    virtual void Start() override; 

    /**
     * @brief Set the On Destory object
     * 
     */
    void SetOnDestory(const OnEventCallback& cb);
protected:

    OnEventCallback m_ondestory_cb;
};

};

}// namespace end