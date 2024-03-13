#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <engine/scene/Scene.hpp>
#include <engine/ecs/gameobject/GameObject.hpp>
#include <share/ecs/gameobject/GameObject.hpp>
#include <share/ecs/Define.hpp>

BOOST_AUTO_TEST_SUITE(GameObjMgrTest)

inline engine::ecs::GameObjectSPtr Create()
{
    return G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
}

BOOST_AUTO_TEST_CASE(t_test_create)
{
    auto object1 = Create();
}

/* 挂载子对象和卸载子对象 */
BOOST_AUTO_TEST_CASE(t_test_mount)
{
    auto object1 = Create();
    auto object2 = Create();
    auto object3 = Create();
    auto object4 = Create();
    auto object5 = Create();
    auto object6 = Create();
    auto object7 = Create();

    object1->MountChild(object2);
    object1->MountChild(object3);
    object2->MountChild(object4);
    object2->MountChild(object5);
    object3->MountChild(object6);
    object3->MountChild(object7);


    auto info = object1->Dbg_GameObjectTree();
    printf("%s\n", info.c_str());
   
}

BOOST_AUTO_TEST_SUITE_END()