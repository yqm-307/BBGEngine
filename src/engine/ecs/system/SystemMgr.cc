#include <engine/ecs/gameobject/GameObjectMgr.hpp>
#include <engine/ecs/system/SystemMgr.hpp>
#include <engine/ecs/filter/EntityFilter.hpp>

namespace engine::ecs
{

SystemMgr::SystemMgr(SceneSPtr scene)
    :m_scene(scene)
{
}

SystemMgr::~SystemMgr()
{
    m_systems.clear();
}

void SystemMgr::RegistSystem(SystemSPtr system, EntityFilter filter)
{
    system->m_filter->AddTag(filter.GetTags());
    m_systems.push_back(system);
}

void SystemMgr::Update()
{
    for (auto& system : m_systems)
    {
        if (system->m_filter == nullptr)
            continue;

        // 只会执行一次
        system->Init();

        if (m_scene.expired())
            return;

        if (GetScene()->GetGameObjectMgr()->GetGameobjectByFilter(system->m_gameobjects, system->m_filter) == 0)
            continue;

        system->Update();
    }
}

} // namespace engine::ecs