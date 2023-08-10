#pragma once
#include "util/network/IOCallbacks.hpp"
#include "util/assert/Assert.hpp"
#include <functional>
#include <thread>

namespace game::util::network
{

class IOThread
{
public:
    typedef std::function<void()> WorkCallback;

    /**
     * @brief 构造IO线程
     * 
     * @param work_cb 工作函数
     */
    IOThread(const WorkCallback& work_cb):m_work_callback(work_cb){}
    virtual ~IOThread() = 0;

    /**
     * @brief 启动线程
     */
    virtual void Start() = 0;
protected:
    WorkCallback    m_work_callback;
    std::thread*    m_thread{nullptr};
};


IOThread::IOThread(const WorkCallback& work_cb)
    :m_work_callback(work_cb)
{
    AssertWithInfo(work_cb != nullptr, "work callback can`t nullptr!");
}

}// namespace end