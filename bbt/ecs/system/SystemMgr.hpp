#pragma once
#include <bbt/ecs/system/System.hpp>


namespace bbt::ecs
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

} // namespace bbt::ecs