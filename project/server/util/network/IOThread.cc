#include "util/network/IOThread.hpp"

namespace game::util::network
{


IOThread::~IOThread() {}

void IOThread::Start()
{
    bool current_flag = m_entrant_already.load();
    m_entrant_already.compare_exchange_strong(current_flag, true);
    AssertWithInfo(current_flag, "io thread not reentrant!");
    AssertWithInfo(m_work_callback != nullptr, "io thread init error!");
    m_work_callback();
    GAME_BASE_LOG_INFO("IOThread exit!");
}

void IOThread::SetWorkTask(const WorkCallback& cb)
{
    m_work_callback = cb;
}


}