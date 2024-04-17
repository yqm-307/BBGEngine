#pragma once
#include <bbt/base/timer/TimeWheel.hpp>
#include <engine/ecs/component/Component.hpp>

namespace share::ecs::timewheel
{

typedef bbt::timer::TimeWheel::TimerSPtr    TaskPtr;
typedef bbt::timer::TimeWheel::TimerId      TaskId;

class TimeWheelComp:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    ~TimeWheelComp() {}
    std::pair<std::optional<bbt::timer::Errcode>, TaskId> AddTask(const std::function<bool()>& timeout_handle, int nframes);
    bool    CancelTask(TaskId task);
protected:
    /**
     * @param frames 每秒tick次数
     */
    TimeWheelComp(int frames);
    virtual void OnUpdate() override;
private:
    bbt::timer::TimeWheel       m_wheel;
    const int                   m_one_second_frames{0};
};

}