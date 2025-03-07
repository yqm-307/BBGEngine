#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/core/random/Random.hpp>

#include "plugin/ecs/globalluavm/GlobalLuaVM.hpp"
#include "plugin/scene/testscene/SampleScene.hpp"
#include "plugin/ecs/entity/Entity.hpp"

#define SHARE_LUA_PATH "../../../plugin/script"

BOOST_AUTO_TEST_SUITE(LuaGlobalVMComponentTest)

int open = 1;

BOOST_AUTO_TEST_CASE(preload)
{
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &open, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
}

BOOST_AUTO_TEST_CASE(t_scene_test)
{
    // 创建场景、空对象，将空对象挂载到场景上
    share::scene::SampleScene m_sample_scene;
    auto object     = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    BOOST_ASSERT(m_sample_scene.MountGameObject(object));

    // 挂载组件到空对象上
    auto component  = G_ComponentMgr()->Create<share::ecs::globalluavm::GlobalLuaVM>();
    BOOST_ASSERT(object->AddComponent(component));

    m_sample_scene.Update();
}

BOOST_AUTO_TEST_CASE(t_do_script_test)
{
    // 创建场景、空对象，将空对象挂载到场景上
    share::scene::SampleScene m_sample_scene;
    auto object     = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    BOOST_ASSERT(m_sample_scene.MountGameObject(object));

    // 挂载组件到空对象上
    auto component  = G_ComponentMgr()->Create<share::ecs::globalluavm::GlobalLuaVM>();
    BOOST_ASSERT(object->AddComponent(component));

    BOOST_ASSERT(component->AddRequirePath("../?.lua"));
    BOOST_ASSERT(component->AddRequirePath("../../../plugin/script/?.lua"));
    BOOST_ASSERT(component->PreloadLuaFile(SHARE_LUA_PATH));


    component->DoScript(R"(
        local _ComponentMgr = require("preload.ComponentMgr")

        _ComponentMgr:RegistComponent(1, function (Args)
            print("on update, id", 1)
        end)
    )");

    component->DoScript(R"(
        local _ComponentMgr = require("preload.ComponentMgr")
        _ComponentMgr:OnUpdateEvent(1)
    )");

    m_sample_scene.Update();
}

BOOST_AUTO_TEST_SUITE_END()