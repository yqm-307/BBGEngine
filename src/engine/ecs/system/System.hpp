#pragma once
#include <engine/ecs/entity/Entity.hpp>

namespace engine::ecs
{

class System
{
    friend class SystemMgr;
public:
    typedef engine::ecs::EntitySPtr EntitySPtr;
    System();
    virtual ~System() = default;

    virtual void                OnInitFilter(std::shared_ptr<engine::ecs::EntityFilter> filter) {}
    virtual void                OnUpdate() = 0;
private:
    virtual void                Update();
    virtual void                Init();
protected:
    std::vector<EntityWKPtr>    m_gameobjects;
    std::shared_ptr<EntityFilter>   m_filter{nullptr};
    std::once_flag                  m_init_flag;
};

}