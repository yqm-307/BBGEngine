#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>


#include <share/scene/TestScene.hpp>
#include <share/ecs/entity/testobj/SimpleObj_1.hpp>



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
    using namespace share::ecs::entity;
    const int SCENE_MAX = 100;
    const int SUB_GAMEOBJ_NUM = 10;
    std::vector<std::shared_ptr<testobj::SimpleObj_1>> vec_testobj;
    share::scene::TestScene scene;

    for(int i = 0; i< SUB_GAMEOBJ_NUM; ++i) {
        vec_testobj.push_back(testobj::SimpleObj_1::FastCreate());
    }
    

    for(auto obj_ptr : vec_testobj) 
    {
        bool isok = scene.MountGameObject(std::static_pointer_cast<engine::ecs::GameObject>(obj_ptr));
        BOOST_CHECK_MESSAGE(isok, "gameobj id=" << obj_ptr->GetId());
    }

    for(int i = 0; i<SCENE_MAX; ++i)
    {
        scene.Update();
        
        for(auto obj_ptr : vec_testobj)
        {
            int obj_update_times = obj_ptr->AloneUpdateTimes();
            BOOST_CHECK_MESSAGE(obj_update_times == (i + 1), "id=" << obj_ptr->GetId() << "\ttimes=" << obj_update_times << "\tcurrent times=" << i);
        }
    }

    auto total_sub_gameobj_update_times = testobj::SimpleObj_1::AllUpdateTimes();
    BOOST_CHECK(total_sub_gameobj_update_times == SCENE_MAX * SUB_GAMEOBJ_NUM);
    BOOST_CHECK(scene.GetUpdateTimes() == SCENE_MAX);
}

// 场景下有一些 GameObject 且 GameObject 也有一些子GameObject 一起 Update
BOOST_AUTO_TEST_CASE(t_scene_has_gameobj_tree_update)
{
    // TODO 需要实现案例
    BOOST_WARN_MESSAGE(false, "尚未实现测试案例");
}

BOOST_AUTO_TEST_SUITE_END()