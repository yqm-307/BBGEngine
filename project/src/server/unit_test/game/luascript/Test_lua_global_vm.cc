#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/base/random/Random.hpp>

#include "share/ecs/globalluavm/GlobalLuaVM.hpp"
#include "share/scene/testscene/SampleScene.hpp"
#include "share/ecs/gameobject/GameObject.hpp"

#define SHARE_LUA_PATH "../../../share/script"

BOOST_AUTO_TEST_SUITE(LuaGlobalVMComponentTest)

BOOST_AUTO_TEST_CASE(t_scene_load_g_lua_vm)
{
    // 创建场景、空对象，将空对象挂载到场景上
    share::scene::SampleScene m_sample_scene;
    auto object     = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    BOOST_ASSERT(m_sample_scene.MountGameObject(object));

    // 挂载组件到空对象上
    auto component  = G_ComponentMgr()->Create<share::ecs::globalluavm::GlobalLuaVM>();
    BOOST_ASSERT(component->PreloadLuaFile(SHARE_LUA_PATH));
    BOOST_ASSERT(object->AddComponent(component));

    m_sample_scene.Update();
}

BOOST_AUTO_TEST_SUITE_END()