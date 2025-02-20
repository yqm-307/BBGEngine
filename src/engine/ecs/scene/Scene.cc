#include <engine/ecs/scene/Scene.hpp>
#include <engine/ecs/component/ComponentMgr.hpp>
#include <engine/ecs/system/SystemMgr.hpp>
#include <engine/ecs/filter/EntityFilter.hpp>

namespace engine::ecs
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
    m_gameobj_mgr = std::make_shared<GameObjectMgr>(shared_from_this());
}

void Scene::RegistSystem(SystemSPtr system, const EntityFilter& filter)
{
    m_system_mgr->RegistSystem(system, filter);
}

void Scene::Update()
{
    m_component_mgr->Update();
    m_gameobj_mgr->Update();
    m_system_mgr->Update();
}

} // namespace engine::ecs