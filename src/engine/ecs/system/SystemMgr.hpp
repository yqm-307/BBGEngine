#pragma once
#include <engine/ecs/system/System.hpp>


namespace engine::ecs
{

class SystemMgr final
{
public:
    SystemMgr(SceneSPtr scene);
    ~SystemMgr();

    void RegistSystem(SystemSPtr system, EntityFilter filter);
    void Update();
    SceneSPtr GetScene() const { return m_scene.lock(); }
private:
    std::vector<SystemSPtr> m_systems;
    SceneWKPtr m_scene;
};

} // namespace engine::ecs