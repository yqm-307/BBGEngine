#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "share/scene/testscene/SampleScene.hpp"
#include "share/ecs/timewheel/TimeWheel.hpp"
#include "share/ecs/timewheel/TimeWheelSystem.hpp"

int open_dbg = 1;

BOOST_AUTO_TEST_SUITE(TimeWheelCompTest)

BOOST_AUTO_TEST_CASE(preload)
{
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &open_dbg, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
}

// 定时器功能测试
BOOST_AUTO_TEST_CASE(t_timewheel_base_test)
{
    int one_sec_frames = 20;
    auto begin_time = bbt::timer::clock::now();
    int num = 0;

    auto scene = share::scene::SampleScene();
    auto gameobject = G_GameObjectMgr()->Create<share::ecs::timewheel::TimeWheel>(one_sec_frames);
    scene.MountGameObject(gameobject);
    share::ecs::timewheel::TimeWheelSystem::GetInstance()->InitGameobject(gameobject, one_sec_frames);
    share::ecs::timewheel::TimeWheelSystem::GetInstance()->AddTask(gameobject,
    [&num](){
        if (num < 10) {
            num++;
            return true;
        }

        return false;
    },
    1);

    for (int i = 0; i < 100; ++i)
    {
        begin_time = begin_time + bbt::timer::clock::ms(10);
        std::this_thread::sleep_until(begin_time);
        scene.Update();
    }

    BOOST_CHECK_EQUAL(num, 10);
}

BOOST_AUTO_TEST_CASE(t_timewheel_cancel_test)
{
    int one_sec_frames = 20;
    int interval = 1000 / one_sec_frames;
    auto begin_time = bbt::timer::clock::now();
    int num = 0;

    auto scene = share::scene::SampleScene();
    auto gameobject = G_GameObjectMgr()->Create<share::ecs::timewheel::TimeWheel>(one_sec_frames);
    scene.MountGameObject(gameobject);

    share::ecs::timewheel::TimeWheelSystem::GetInstance()->InitGameobject(gameobject, one_sec_frames);
    auto [err, taskid] = share::ecs::timewheel::TimeWheelSystem::GetInstance()->AddTask(gameobject,
    [&](){
        if (num < 10) {
            num++;
            return true;
        }

        return false;
    },
    2);

    BOOST_ASSERT(err == std::nullopt);

    scene.Update();
    share::ecs::timewheel::TimeWheelSystem::GetInstance()->CancelTask(gameobject, taskid);

    for (int i = 0; i < 10; ++i)
    {
        begin_time = begin_time + bbt::timer::clock::ms(interval);
        std::this_thread::sleep_until(begin_time);
        scene.Update();
    }

    BOOST_CHECK_EQUAL(num, 0);
}


BOOST_AUTO_TEST_SUITE_END()