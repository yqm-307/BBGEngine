#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>


#include <share/scene/TestScene.hpp>
#include <share/ecs/entity/testobj/SimpleObj_1.hpp>
#include <cmath>

/**
 * @brief 场景update测试夹具，用来清空update计数
 * 
 */
struct SceneFixture
{
    SceneFixture(){}
    ~SceneFixture(){ share::ecs::entity::testobj::SimpleObj_1::ResetAllUpdateTimes(); }
};



BOOST_AUTO_TEST_SUITE(SceneTest)

// 单独场景 Update
BOOST_FIXTURE_TEST_CASE(t_scene_alone_update, SceneFixture)
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
BOOST_FIXTURE_TEST_CASE(t_scene_has_gameobj_update, SceneFixture)
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


void MountObj(engine::ecs::GameObjectSPtr root, int child_num, int depth)
{
    if(depth <= 0)
        return;

    for(int i = 0; i < child_num; ++i)
    {
        auto obj = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::entity::testobj::SimpleObj_1>();
        auto isok = root->MountChild(obj);
        BOOST_CHECK(isok);
        MountObj(obj, child_num, depth - 1);
    }
}

int64_t Factorial(uint32_t num) {
    if(num == 0 || num == 1) return 1;
    return num*Factorial(num - 1);
}

// level 层 sub 叉树不带根节点的节点数
int ChildObjNum(int level, int sub)
{
    return ( pow(sub, level) - 1)/(sub - 1) - 1;
}

// 场景下有一些 GameObject 且 GameObject 也有一些子GameObject 一起 Update
BOOST_FIXTURE_TEST_CASE(t_scene_has_gameobj_tree_update, SceneFixture)
{
    using namespace share::ecs::entity;

    const int SCENE_MAX = 100;  // 
    const int MAX_LEVEL = 2;    // 4 层
    const int SUB_GAMEOBJ_NUM = 3;  // 每个子节点有多少个对象

    share::scene::TestScene scene;


    for(int i = 0; i < SUB_GAMEOBJ_NUM; ++i)
    {
        auto obj = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::entity::testobj::SimpleObj_1>();
        auto isok = scene.MountGameObject(obj);
        BOOST_CHECK(isok);
        MountObj(obj, SUB_GAMEOBJ_NUM, MAX_LEVEL - 1);
    }

    for(int i = 0; i < SCENE_MAX; ++i)
    {
        scene.Update();
    }

    BOOST_CHECK_MESSAGE( share::ecs::entity::testobj::SimpleObj_1::AllUpdateTimes() == (SCENE_MAX * ChildObjNum(MAX_LEVEL + 1, SUB_GAMEOBJ_NUM)) ,
        "SimpleObj_1 num=" << (int64_t)ChildObjNum(MAX_LEVEL + 1, SUB_GAMEOBJ_NUM) << 
        "\ttotal update times=" << share::ecs::entity::testobj::SimpleObj_1::AllUpdateTimes() << 
        "\tscene update times=" << SCENE_MAX);

}

BOOST_AUTO_TEST_SUITE_END()