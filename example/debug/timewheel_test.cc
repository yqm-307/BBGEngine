#include "plugin/scene/testscene/SampleScene.hpp"
#include "plugin/ecs/timewheel/TimeWheel.hpp"
#include "plugin/ecs/timewheel/TimeWheelSystem.hpp"

// 检测是否丢失、精准率
struct Ctrl
{
};

// 定时器功能测试
int main()
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