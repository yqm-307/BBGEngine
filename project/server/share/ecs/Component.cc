#include "share/ecs/Component.hpp"

using namespace game::share::ecs;

const std::string& Component::GetName() const
{
    return m_name;
}

void Component::SetName(std::string component)
{
    m_name = component;
}