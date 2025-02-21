#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/base/random/Random.hpp>

#include "plugin/ecs/entity/Entity.hpp"
#include "plugin/ecs/luascript/LuaTestComp.hpp"
#include "plugin/ecs/globalluavm/GlobalLuaVM.hpp"
#include "plugin/scene/testscene/SampleScene.hpp"

BOOST_AUTO_TEST_SUITE(LuaScriptComponentTest)

int open = 1;

BOOST_AUTO_TEST_CASE(preload)
{
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &open, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
}

BOOST_AUTO_TEST_CASE(t_scene_load_lua_files)
{
    share::scene::SampleScene m_sample_scene;

    auto vm_component = G_ComponentMgr()->Create<share::ecs::globalluavm::GlobalLuaVM>();
    BOOST_ASSERT(vm_component->AddRequirePath("../?.lua"));
    BOOST_ASSERT(vm_component->AddRequirePath("../../../plugin/script/?.lua"));
    BOOST_ASSERT(vm_component->PreloadLuaFile("../../../plugin/script"));
    BOOST_ASSERT(vm_component->LoadLuaLibrary());

    auto lua_object = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto lua_script_comp = G_ComponentMgr()->Create<share::ecs::luascript::LuaTestComp>(vm_component->GetVM());
    BOOST_ASSERT(lua_script_comp->Init("main.lua"));

    lua_script_comp->DoScript(R"(
        G_UPDATE_COUNT = 0
    )");

    BOOST_ASSERT(lua_object->AddComponent(lua_script_comp));
    BOOST_ASSERT(m_sample_scene.MountGameObject(lua_object));

    m_sample_scene.Update();    // 计数 +1
    m_sample_scene.UnMountGameObject(lua_object->GetId());
    m_sample_scene.Update();
    m_sample_scene.MountGameObject(lua_object);
    m_sample_scene.Update();    // 计数 +1

    bbt::cxxlua::LuaValue value;
    lua_script_comp->GetGlobal(value, "G_UPDATE_COUNT");
    
    int integer;
    auto err = value.GetValue(integer);
    BOOST_ASSERT(value.GetType() == bbt::cxxlua::LUATYPE::LUATYPE_NUMBER);
    BOOST_CHECK_EQUAL(integer, 2);
}

BOOST_AUTO_TEST_SUITE_END()