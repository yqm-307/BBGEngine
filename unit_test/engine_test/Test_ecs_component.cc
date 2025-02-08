#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <plugin/scene/testscene/SampleScene.hpp>
#include <plugin/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/testobj/EmptyComp.hpp>

// ecs 的 component 基准测试
BOOST_AUTO_TEST_SUITE(ComponentTest)

// component 基础功能测试
BOOST_AUTO_TEST_CASE(t_basic_operation)
{
    // 测试挂载时组件保存父节点
    auto scene = share::scene::SampleScene();
    auto gameobject = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto empty_comp = G_ComponentMgr()->Create<share::ecs::component::EmptyComponent>();

    BOOST_ASSERT( scene.MountGameObject(gameobject) );
    BOOST_ASSERT( gameobject->AddComponent(empty_comp));
    auto empty_parent_object = empty_comp->GetParentObject();
    BOOST_CHECK_EQUAL( empty_parent_object, gameobject );
    auto drop_comp = gameobject->DelComponent(empty_comp->GetTemplateId());
    BOOST_CHECK_EQUAL( empty_comp, drop_comp );
    BOOST_CHECK_EQUAL( nullptr, empty_comp->GetParentObject());
}

BOOST_AUTO_TEST_SUITE_END()