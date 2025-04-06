#include <bbt/ecs/scene/Scene.hpp>
#include <bbt/ecs/component/ComponentMgr.hpp>
#include <bbt/ecs/system/SystemMgr.hpp>
#include <bbt/ecs/filter/EntityFilter.hpp>

namespace bbt::ecs
{


Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
    m_component_mgr = std::make_shared<ComponentMgr>(shared_from_this());
    m_system_mgr = std::make_shared<SystemMgr>(shared_from_this());
    m_entity_mgr = std::make_shared<EntityMgr>(shared_from_this());
}

void Scene::RegistSystem(SystemSPtr system, const EntityFilter& filter)
{
    m_system_mgr->RegistSystem(system, filter);
}

void Scene::Update()
{
    m_component_mgr->Update();
    m_entity_mgr->Update();
    m_system_mgr->Update();
}

} // namespace bbt::ecs