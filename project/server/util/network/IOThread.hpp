#pragma once
#include <bbt/poolutil/IDPool.hpp>
#include <functional>
#include <thread>
#include <any>
#include "util/network/IOCallbacks.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"

namespace game::util::network
{

enum IOThreadRunStatus
{
    Default = 0,
    Running = 1,
    Finish = 2,
};

/* io线程id，小于0是非法值 */
typedef int IOThreadID;

class IOThread
{
public:
    typedef std::function<void()> WorkCallback;
    typedef std::function<void(IOThreadID)> HookCallback;

    /**
     * @brief 构造IO线程
     * 
     * @param work_cb 工作函数
     */
    IOThread();
    virtual ~IOThread();

    /* （对外接口）需要子类实现: 调用此函数开始让工作线程开始工作 */
    virtual void Start() = 0;

    /* （对外接口）需要子类实现: 调用此函数阻塞的等待 work 线程调用完毕 */
    virtual void Stop() = 0;

    /* 获取 io thread 的线程id，非系统id。常规情况使用此为IOThread的标识*/
    virtual IOThreadID GetTid() const final;

    /* 线程是否正在运行 */
    bool IsRunning() const;
protected:    
    /* 获取 Sys thread id ，慎用，通常不作为IOThread的标识*/
    std::thread::id GetSysTid() const;

    /* （内部接口）启动线程 */
    void StartWorkFunc();
    
    /* 线程运行开始之前的 hook 函数 */
    void SetOnThreadBegin_Hook(const HookCallback& cb);
    
    /* 线程运行结束之后的 hook 函数 */
    void SetOnThreadEnd_Hook(const HookCallback& cb);
    
    /* 设置工作函数，IOThread开始的时候会调用 */
    void SetWorkTask(const WorkCallback& cb);
protected:
    /* 由子类控制 */
    IOThreadRunStatus   m_status{IOThreadRunStatus::Default};
private:
    void Init();
    void Destory();
    /* 核心函数，真正的work线程 */
    void Work();
private:
    WorkCallback    m_work_callback{nullptr};
    HookCallback    m_thread_start_before_callback{nullptr};
    HookCallback    m_thread_stop_after_callback{nullptr};
    std::thread*    m_thread{nullptr};
    IOThreadID      m_tid{-1};
    volatile bool   m_running{false};
    /* 其他 */
    static bbt::pool_util::IDPool<int, true>    m_id_pool;
};



}// namespace end