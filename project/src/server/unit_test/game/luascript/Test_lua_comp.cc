#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/base/random/Random.hpp>

#include "share/ecs/gameobject/GameObject.hpp"
#include "share/ecs/luascript/LuaScriptComponent.hpp"
#include "share/ecs/globalluavm/GlobalLuaVM.hpp"
#include "share/scene/testscene/SampleScene.hpp"

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
    BOOST_ASSERT(vm_component->AddRequirePath("../../../share/script/?.lua"));
    BOOST_ASSERT(vm_component->PreloadLuaFile("../../../share/script"));
    BOOST_ASSERT(vm_component->LoadLuaLibrary());

    auto lua_object = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto lua_script_comp = G_ComponentMgr()->Create<share::ecs::luascript::LuaScriptComponent>(vm_component->GetVM(), "main.lua");
    BOOST_ASSERT(lua_script_comp->IsInitSucc());

    BOOST_ASSERT(lua_object->AddComponent(lua_script_comp));
    BOOST_ASSERT(m_sample_scene.MountGameObject(lua_object));

    m_sample_scene.Update();
    m_sample_scene.UnMountGameObject(lua_object->GetId());
    m_sample_scene.Update();
    m_sample_scene.MountGameObject(lua_object);
    m_sample_scene.Update();
}

BOOST_AUTO_TEST_SUITE_END()