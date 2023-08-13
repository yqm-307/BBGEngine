#include "util/network/IOThread.hpp"

namespace game::util::network
{

static const int io_thread_limit_num = 16;

bbt::pool_util::IDPool<int, true> IOThread::m_id_pool = bbt::pool_util::IDPool<int, true>(io_thread_limit_num);  // todo: thread limit config

IOThread::IOThread()
{
    Init();
}

IOThread::~IOThread() 
{
    Destory();
}

void IOThread::Init()
{
    auto [isok, id] = m_id_pool.GetID();
    AssertWithInfo(isok, "maybe thread num range out of thread limit!");
    m_tid = id;
}

void IOThread::Destory()
{
    auto isok = m_id_pool.ReleaseID(m_tid);
    AssertWithInfo(isok, "this is a fatal bug! maybe is a bug with id pool!");
    m_tid = -1;

    m_thread_start_before_callback = nullptr;
    m_thread_stop_after_callback = nullptr;

}

void IOThread::SetWorkTask(const WorkCallback& cb)
{
    m_work_callback = cb;
}

IOThreadID IOThread::GetTid() const
{
    return m_tid;
}

std::thread::id IOThread::GetSysTid() const
{
    return m_thread->get_id();
}

void IOThread::StartWorkFunc()
{
    DebugAssertWithInfo(m_thread == nullptr,        "this is abnormal behavior");
    DebugAssertWithInfo(m_work_callback != nullptr, "this is a fatal bug! work thread is exist!");

    m_thread = new std::thread([=](){
        Work();
    });
}

void IOThread::SetOnThreadBegin_Hook(const HookCallback& cb)
{
    m_thread_start_before_callback = cb;
}
void IOThread::SetOnThreadEnd_Hook(const HookCallback& cb)
{
    m_thread_stop_after_callback = cb;
}

void IOThread::Work()
{
    
    /* 尝试执行线程开始前的 */
    if(m_thread_start_before_callback)
        m_thread_start_before_callback(m_tid);

    m_status = IOThreadRunStatus::Running;
    
    m_work_callback();

    m_status = IOThreadRunStatus::Finish;

    if(m_thread_stop_after_callback)
        m_thread_stop_after_callback(m_tid);
}

bool IOThread::IsRunning() const
{
    return (m_status == IOThreadRunStatus::Running);
}


}