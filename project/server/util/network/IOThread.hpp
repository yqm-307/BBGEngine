#pragma once
#include <bbt/base/poolutil/IDPool.hpp>
#include <functional>
#include <thread>
#include <any>
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"

namespace util::network
{

enum IOThreadRunStatus
{
    Default = 0,
    Running = 1,
    Finish = 2,
};

/* io线程id，小于0是非法值 */
typedef int IOThreadID;

/**
 * @brief IOThread是什么？是对抽象的IO线程的美好描绘
 * 
 * Q: IOThread其实可以拿来做其他的事情，比如非IO事件。那么看来叫Thread更好？
 * A: 不是的，在游戏里面，如果是传统的网游，都是单逻辑线程 + 多IO线程，所以
 * 游戏内一般不需要多开线程了。
 * 
 * Q: IOThread做了什么？
 * A: 一个线程的基本标识（tid）、线程真正启动时的流程（封装起来会好）。一个继承
 * 自IOThread的类需要继承并做一些事情就可以轻松的启动和关闭了，还支持了Hook。
 * 
 * Q: 继承需要注意什么？
 * A: 重写Start、Stop方法，基本上是想让继承者自己控制线程如何启动，以及如何停止
 * ，因为通常停止都是一个跨线程操作，所以需要继承者去做。其次就是启动前要设置工作
 * 函数（SetWorkTask），启动时不需要重写启动逻辑，而是在做一些操作后调用IOThread
 * 已经处理好的线程启动逻辑（这个是必须的）.
 */
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
    virtual void Start();

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

    /* 阻塞的等待线程退出 */
    void SyncWaitThreadExit();

    /* 阻塞的等待线程退出，阻塞到wait_time就返回，wait_time小于0则无限阻塞，等于0则不阻塞 */
    void SyncWaitThreadExitWithTime(int wait_time);
protected:
    /* 由子类控制 */
    IOThreadRunStatus   m_status{IOThreadRunStatus::Default};
private:
    void Init();
    void Destory();
    /* 核心函数，真正的work线程 */
    void Work();
    void SyncWaitThreadExitEx(int wait_time);
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