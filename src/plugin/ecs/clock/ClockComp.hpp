#pragma once
#include <bbt/base/clock/Clock.hpp>
#include <engine/ecs/component/Component.hpp>
#include <engine/ecs/EcsDefine.hpp>


namespace plugin::ecs::clock
{

class ClockComp:
    public engine::ecs::Component
{
    ComponentDeriveClassDef(EM_COMPONENT_TYPE_CLOCK_MODULE)
public:
    virtual ~ClockComp() {}

    void SetFrameInterval(int interval_ms);
    int  GetFrameInterval();

    /**
     * @brief 获取当前帧
     * 
     * @return uint64_t 
     */
    int64_t GetFrame();

    /**
     * @brief 获取当前帧的时间
     * 
     * @return bbt::clock::Timestamp<> 
     */
    bbt::clock::Timestamp<> GetFrameTime();
protected:
    ClockComp();
    virtual void OnUpdate();
private:
    int                     m_interval_ms{0};
    int64_t                m_total_pass_frame{0};   // 帧
    bbt::clock::Timestamp<> m_last_frame_update_timestamp; // 帧时间
};

}