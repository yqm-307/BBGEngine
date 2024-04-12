#pragma once
#include <bbt/base/timer/TimeWheel.hpp>
#include <engine/ecs/component/Component.hpp>

namespace share::ecs::timewheel
{

typedef std::function<void()> TimerHandle;
typedef bbt::timer::TimeWheel<TimerHandle>::TaskBasePtr TaskPtr;
typedef bbt::timer::TimeWheel<TimerHandle>::TaskID TaskId;

class TimeWheelComp:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    ~TimeWheelComp() {}
    TaskId AddTask(const std::function<bool()>& timeout_handle, int nframes);
    bool CancelTask(TaskId task);
protected:
    /**
     * @param frames 每秒tick次数
     */
    TimeWheelComp(int frames);
    virtual void OnUpdate() override;
private:
    bbt::timer::TimeWheel<TimerHandle> m_wheel;
    const int m_one_second_frames{0};
};

}