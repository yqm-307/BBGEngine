#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/clock/ClockComp.hpp"

namespace plugin::ecs::clock
{

ClockComp::ClockComp()
{
}

void ClockComp::OnUpdate()
{
    auto now_timestamp = bbt::core::clock::now<>();
    int64_t pass_time = (now_timestamp - m_last_frame_update_timestamp).count();
    if (m_interval_ms <= pass_time)
    {
        m_last_frame_update_timestamp = now_timestamp;
        m_total_pass_frame = m_total_pass_frame + 1;
    }
}

void ClockComp::SetFrameInterval(int interval_ms)
{
    m_interval_ms = interval_ms;
}

int ClockComp::GetFrameInterval()
{
    return m_interval_ms;
}

int64_t ClockComp::GetFrame()
{
    return m_total_pass_frame;
}

bbt::core::clock::Timestamp<> ClockComp::GetFrameTime()
{
    return m_last_frame_update_timestamp;
}

}