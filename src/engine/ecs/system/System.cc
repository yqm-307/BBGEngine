#include <engine/ecs/system/System.hpp>
#include <engine/ecs/filter/EntityFilter.hpp>

namespace engine::ecs
{

System::System():
    m_filter(std::make_shared<EntityFilter>())
{
}

void System::Update()
{
    
    OnUpdate();
}

void System::Init()
{
    std::call_once(m_init_flag, [this](){
        OnInitFilter(m_filter);
    });
}

}