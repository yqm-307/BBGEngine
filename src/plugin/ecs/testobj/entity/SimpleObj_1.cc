#include "plugin/ecs/testobj/entity/SimpleObj_1.hpp"
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::entity::testobj
{

int SimpleObj_1::create_num = 0;
int SimpleObj_1::destory_num = 0;
int SimpleObj_1::update_num = 0;

std::shared_ptr<SimpleObj_1> SimpleObj_1::FastCreate()
{
    auto ptr = G_GameObjectMgr()->Create<SimpleObj_1>();
    Assert(ptr != nullptr);
    return ptr;
}


SimpleObj_1::SimpleObj_1()
    :engine::ecs::GameObject(plugin::ecs::emEntityType::EM_ENTITY_TYPE_TESTOBJ_1)
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

void SimpleObj_1::ResetAllUpdateTimes()
{
    update_num = 0;
}


int SimpleObj_1::AloneUpdateTimes()
{
    return m_owner_update_num;
}

} // namespace share::ecs::entity::testobj
