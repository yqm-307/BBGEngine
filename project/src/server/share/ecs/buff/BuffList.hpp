#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/buff/BuffBase.hpp"


namespace share::ecs::buff
{

class BuffList:
    public engine::ecs::Component
{
private:
    virtual void OnUpdate() override;

private:
    std::vector<std::shared_ptr<BuffBase>> m_buff_array;
};

}