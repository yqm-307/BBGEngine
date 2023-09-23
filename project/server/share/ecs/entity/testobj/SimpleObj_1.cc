#include "share/ecs/entity/testobj/SimpleObj_1.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::testobj
{

int SimpleObj_1::create_num = 0;
int SimpleObj_1::destory_num = 0;
int SimpleObj_1::update_num = 0;

SimpleObj_1::SimpleObj_1()
    :engine::ecs::GameObject(share::ecs::emEntityType::EM_ENTITY_TYPE_TESTOBJ_1)
{

}

SimpleObj_1::~SimpleObj_1()
{
    destory_num--;
}

void SimpleObj_1::OnUpdate()
{
    m_owner_update_num++;
    update_num++;
}

int SimpleObj_1::AllUpdateTimes()
{
    return update_num;
}

int SimpleObj_1::AloneUpdateTimes()
{
    return m_owner_update_num;
}

} // namespace share::ecs::entity::testobj
