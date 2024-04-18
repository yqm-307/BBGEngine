#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "share/scene/testscene/SampleScene.hpp"
#include "share/ecs/timewheel/TimeWheel.hpp"
#include "share/ecs/timewheel/TimeWheelSystem.hpp"

int open_dbg = 1;

BOOST_AUTO_TEST_SUITE(TimeWheelFatigueTest)

BOOST_AUTO_TEST_CASE(preload)
{
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &open_dbg, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
}

// 检测是否丢失、精准率
struct Ctrl
{
};

// 定时器功能测试
BOOST_AUTO_TEST_CASE(t_timewheel_fatigue_test)
{
    int one_sec_frames = 20;
    auto begin_time = bbt::timer::clock::now();
    int num = 0;


    auto scene = share::scene::SampleScene();
    auto gameobject = G_GameObjectMgr()->Create<share::ecs::timewheel::TimeWheel>(one_sec_frames);
    scene.MountGameObject(gameobject);
    share::ecs::timewheel::TimeWheelSystem::GetInstance()->InitGameobject(gameobject, one_sec_frames);

    for (int i = 0; i < 5; ++i) {
        share::ecs::timewheel::TimeWheelSystem::GetInstance()->AddTask(gameobject,
        [&num, i, gameobject](){
            share::ecs::timewheel::TimeWheelSystem::GetInstance()->AddTask(gameobject,
            [](){
                return false;
            }, 1 + i);
            return true;
        },
        1);
    }

    for (;;)
    {
        begin_time = begin_time + bbt::timer::clock::ms(10);
        std::this_thread::sleep_until(begin_time);
        scene.Update();
    }
}


BOOST_AUTO_TEST_SUITE_END()