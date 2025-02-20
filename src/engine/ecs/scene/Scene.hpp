#pragma once
#include <engine/ecs/EcsDefine.hpp>
#include <engine/ecs/component/ComponentMgr.hpp>
#include <engine/ecs/gameobject/GameObjectMgr.hpp>

namespace engine::ecs
{

class Scene final:
    public std::enable_shared_from_this<Scene>
{
public:
    Scene();
    ~Scene();

    void Init();

    template<class TComponent>
    void RegistComponent(const std::string& component_name);

    void RegistSystem(SystemSPtr system, const EntityFilter& filter);

    ComponentMgrSPtr GetComponentMgr() const { return m_component_mgr; }
    SystemMgrSPtr GetSystemMgr() const { return m_system_mgr; }
    GameObjectMgrSPtr GetGameObjectMgr() const { return m_gameobj_mgr; }

    void Update();
private:
    SystemMgrSPtr       m_system_mgr{nullptr};
    ComponentMgrSPtr    m_component_mgr{nullptr};
    GameObjectMgrSPtr   m_gameobj_mgr{nullptr};
};

template<class TComponent>
void Scene::RegistComponent(const std::string& component_name)
{
    m_component_mgr->Register<TComponent>(component_name);
}


} // namespace engine::ecs