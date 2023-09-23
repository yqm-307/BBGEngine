#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <engine/scene/Scene.hpp>
#include <share/ecs/entity/player/Player.hpp>
#include <share/ecs/entity/none/NoneObj.hpp>
using namespace share::ecs::entity;

BOOST_AUTO_TEST_SUITE(GameObjMgrTest)


BOOST_AUTO_TEST_CASE(t_test_create)
{
    auto obj_ptr1 = engine::ecs::GameObjectMgr::GetInstance()->Create<player::Player>();

    /* 第一个创建的id是1 */
    BOOST_CHECK( obj_ptr1 != nullptr );
    BOOST_CHECK( obj_ptr1->GetId() == 1);

    auto obj_ptr2 = engine::ecs::GameObjectMgr::GetInstance()->Create<none::NoneObj>();

    BOOST_CHECK( obj_ptr2 != nullptr );
    BOOST_CHECK( obj_ptr2->GetId() == 2 );

    auto obj_ptr3 = engine::ecs::GameObjectMgr::GetInstance()->Create<player::Player>();

    /* 第一个创建的id是1 */
    BOOST_CHECK( obj_ptr3 != nullptr );
    BOOST_CHECK( obj_ptr3->GetId() == 3);

}

BOOST_AUTO_TEST_SUITE_END()