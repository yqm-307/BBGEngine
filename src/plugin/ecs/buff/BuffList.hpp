#pragma once
#include "engine/ecs/component/Component.hpp"
#include "plugin/ecs/buff/BuffBase.hpp"


namespace plugin::ecs::buff
{

struct OnRealHpChangeParams
{
    int32_t change_value;
};

struct OnEventParams
{
    engine::ecs::GameObjectSPtr     from;  // 释放者
    engine::ecs::GameObjectSPtr     to;    // 目标
    BuffTriggerType                 trigger_type; // 触发事件的类型
    void*                           param;
};

class BuffList:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    virtual ~BuffList() {}
    /**
     * @brief 驱动BuffList的核心函数，通过抛出事件
     *  让buff进行处理。
     *  
     * 例：
     * 抛出OnHurt事件，处理反伤、回复、重伤等效果
     * 
     * @param params 
     */
    void OnEvent(const OnEventParams& params);

    bool AddBuff(BuffBase::SPtr buff);
    bool DelBuff(BuffId id);
    BuffBase::SPtr GetBuffById(BuffId id);
private:
    BuffList();
    virtual void OnUpdate() override;

    void OnCommonEvent(
        engine::ecs::GameObjectSPtr from,
        engine::ecs::GameObjectSPtr to,
        BuffTriggerType type,
        const void* damage_value);

private:
    std::unordered_map<BuffId, BuffBase::SPtr> m_buff_map;
    std::unordered_map<BuffTriggerType, std::unordered_map<BuffId, BuffBase::SPtr>> m_event_map;
};

}