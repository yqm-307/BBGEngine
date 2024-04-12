#include "share/ecs/Define.hpp"
#include "share/ecs/timewheel/TimeWheelComp.hpp"

namespace share::ecs::timewheel
{

TimeWheelComp::TimeWheelComp(int frames):
    engine::ecs::Component(EM_COMPONENT_TYPE_TIME_WHEEL),
    m_one_second_frames(frames)
{
}


void TimeWheelComp::OnUpdate()
{
    m_wheel.Tick();
}

TaskId TimeWheelComp::AddTask(const std::function<bool()>& timeout_handle, int nframes)
{
    int interval = (1000 / m_one_second_frames) * nframes;

    auto task = std::make_shared<bbt::timer::TimeTask_Base<std::function<void()>>>();
    task->Init(
        [=](){
            bool go_on = timeout_handle();
            if (go_on) {
                bool succ = m_wheel.AddTask(task);
                if (!succ)
                    GAME_EXT1_LOG_ERROR("go on regist time task failed! taskid=%d", task->GetTaskID());
            }
        },
        bbt::timer::clock::nowAfter<bbt::timer::clock::ms>(bbt::timer::clock::ms(interval)
    ));

    m_wheel.AddTask(task);
    return task->GetTaskID(); 
}

bool TimeWheelComp::CancelTask(TaskId task)
{
    return m_wheel.CancelTask(task);
}

}