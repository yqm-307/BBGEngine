#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/base/random/Random.hpp>

#include "share/ecs/gameobject/GameObject.hpp"
#include "share/ecs/luascript/LuaScriptComponent.hpp"
#include "share/scene/testscene/SampleScene.hpp"

BOOST_AUTO_TEST_SUITE(LuaScriptComponentTest)

BOOST_AUTO_TEST_CASE(t_scene_load_lua_files)
{
    bbt::cxxlua::LuaVM vm;
    auto load_lib_err = vm.LoadLuaLibrary();
    BOOST_ASSERT(load_lib_err == std::nullopt);

    share::scene::SampleScene m_sample_scene;
    auto lua_object = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto lua_script_comp = G_ComponentMgr()->Create<share::ecs::luascript::LuaScriptComponent>(&vm, "main.lua");
    BOOST_ASSERT(lua_script_comp->IsInitSucc());

    BOOST_ASSERT(lua_object->AddComponent(lua_script_comp));
    BOOST_ASSERT(m_sample_scene.MountGameObject(lua_object));
}

BOOST_AUTO_TEST_SUITE_END()