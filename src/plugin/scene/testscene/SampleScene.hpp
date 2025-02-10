#pragma once
#include "engine/scene/Scene.hpp"

namespace plugin::scene
{

class SampleScene:
    public engine::scene::Scene
{
public:
    explicit SampleScene() {}
    ~SampleScene() {}

private:
    virtual void OnUpdate() override {};
};

} // namespace share::scene