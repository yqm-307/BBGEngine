#include <bbt/base/timer/TimeWheel.hpp>
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/timewheel/TimeWheelComp.hpp"

namespace share::ecs::timewheel
{

TimeWheelComp::TimeWheelComp(int frames):
    engine::ecs::Component(EM_COMPONENT_TYPE_TIME_WHEEL),
    m_one_second_frames(frames)
{
}


void TimeWheelComp::OnUpdate()
{
    /*XXX
    * 不是很精准，最好是每次tick完计算距离下次的时间间隔，然后以此时间间隔注册事件
    */
    while (m_wheel.HasTimeoutSlot(bbt::timer::clock::now()))
    {
        // GAME_BASE_LOG_DEBUG("tick once, %ld, %ld",
        //     m_wheel.GetNextTickTimestamp().time_since_epoch().count(),
        //     bbt::timer::clock::now<>().time_since_epoch().count());
        m_wheel.Tick();
    }
}

std::pair<std::optional<bbt::timer::Errcode>, TaskId> TimeWheelComp::AddTask(const std::function<bool()>& timeout_handle, int nframes)
{
    int interval = (1000 / m_one_second_frames) * nframes;

    auto [err, timerid] = m_wheel.RegistTask(timeout_handle, interval);

    if (err != std::nullopt)
        return {err, 0};
    
    return {std::nullopt, timerid}; 
}

bool TimeWheelComp::CancelTask(TaskId task)
{
    return m_wheel.UnRegistTask(task);
}

}