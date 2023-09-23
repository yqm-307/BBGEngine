#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>


#include <share/scene/TestScene.hpp>

BOOST_AUTO_TEST_SUITE(SceneTest)

// 单独场景 Update
BOOST_AUTO_TEST_CASE(t_scene_alone_update)
{
    const int MAX = 10000;

    share::scene::TestScene scene;

    for(int i = 0; i < MAX; ++i)
    {
        scene.Update();
    }

    BOOST_CHECK( scene.GetUpdateTimes() == MAX );
}

// 场景下有一些 GameObject 一起 Update
BOOST_AUTO_TEST_CASE(t_scene_has_gameobj_update)
{
    // TODO 需要实现案例
    BOOST_WARN_MESSAGE(false, "尚未实现测试案例");
}

// 场景下有一些 GameObject 且 GameObject 也有一些子GameObject 一起 Update
BOOST_AUTO_TEST_CASE(t_scene_has_gameobj_tree_update)
{
    // TODO 需要实现案例
    BOOST_WARN_MESSAGE(false, "尚未实现测试案例");
}

BOOST_AUTO_TEST_SUITE_END()