#pragma once
#include "engine/scene/Scene.hpp"

namespace plugin::scene
{

class TestScene:
    public engine::scene::Scene
{
public:
    explicit TestScene() {}
    ~TestScene() {}

    int GetUpdateTimes();
private:
    virtual void OnUpdate() override;
private:
    int m_update_times{0};
};

} // namespace share::scene