#pragma once
#include "util/network/IOCallbacks.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"
#include <functional>
#include <thread>
#include <any>

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
    IOThread(){}
    virtual ~IOThread() = 0;

    /**
     * @brief 启动线程
     */
    virtual void Start();
    void SetWorkTask(const WorkCallback& cb);
protected:
    WorkCallback    m_work_callback;
    std::thread*    m_thread{nullptr};
    std::atomic_bool    m_entrant_already{false};
};



}// namespace end