#include <bbt/ecs/system/System.hpp>
#include <bbt/ecs/filter/EntityFilter.hpp>

namespace bbt::ecs
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