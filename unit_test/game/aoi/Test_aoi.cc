#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/base/random/Random.hpp>
#include <mutex>
#include "plugin/ecs/aoi/AoiObjectComponent.hpp"
// #include "plugin/ecs/none/entity/NoneObj.hpp"
#include "plugin/ecs/entity/Entity.hpp"
#include "plugin/ecs/aoi/AoiSystem.hpp"
#include "util/vector/CalcOpt.hpp"

using namespace share;
using namespace util;
typedef engine::ecs::EntitySPtr EntitySPtr;
typedef std::shared_ptr<ecs::aoi::AoiObjectComponent> AoiObjComponentPtr;
typedef std::shared_ptr<ecs::gameobject::GameObject> GameobjectPtr;


std::shared_ptr<ecs::aoi::AoiObjectComponent> GetAoiComponet(GameobjectPtr obj)
{
    auto aoi_obj = obj->GetComponent(ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT);
    BOOST_ASSERT(aoi_obj != nullptr);
    return std::static_pointer_cast<ecs::aoi::AoiObjectComponent>(aoi_obj);
}

void AoiNotifyEvent(EntitySPtr aoi, EntitySPtr w, EntitySPtr m , const std::string str)
{

    auto p1 = std::static_pointer_cast<ecs::gameobject::GameObject>(w);
    auto p2 = std::static_pointer_cast<ecs::gameobject::GameObject>(m);
    auto pos1 = GetAoiComponet(p1);
    auto pos2 = GetAoiComponet(p2);

    printf("----------------------------------------------------\n");
    printf("[%s]对象: %d 看到 对象(%d):\n", str.c_str(), pos1->GetObjId(), pos2->GetObjId());
    // pos2->Debug_PosChange();
}

/* 创建一个纯动态的aoi对象 */
EntitySPtr CreateAoiObj()
{
    auto noneobj = G_GameObjectMgr()->Create<ecs::gameobject::GameObject>();
    auto aoi_obj_component = G_ComponentMgr()->Create<ecs::aoi::AoiObjectComponent>(share::ecs::aoi::AoiEntityFlag::Watcher);
    noneobj->AddComponent(aoi_obj_component);
    return noneobj;
}

std::once_flag flag;

util::config::AoiConfig* GenAoiConfig()
{
    static util::config::AoiConfig* aoi = nullptr;
    std::call_once(flag, [&](){
        aoi = new util::config::AoiConfig();
        aoi->m_map_x = 10.0f;
        aoi->m_map_y = 10.0f;
        aoi->m_map_z = 10.0f;
        aoi->m_tower_x = 1.0f;
        aoi->m_tower_y = 1.0f;
        aoi->m_tower_z = 1.0f;
    });

    return aoi;    
}

EntitySPtr CreateAoiMap()
{
    auto noneobj = G_GameObjectMgr()->Create<ecs::gameobject::GameObject>();
    auto aoi_component = G_ComponentMgr()->Create<ecs::aoi::AoiComponent>(GenAoiConfig());
    noneobj->AddComponent(aoi_component);
    return noneobj;
}


BOOST_AUTO_TEST_SUITE(AoiTest)

/* 测试两个对象相遇在一个aoi中 */
BOOST_AUTO_TEST_CASE(t_aoi_2_object_meet)
{
    std::vector<util::pos::Point3> pos_set1 = {
        {0.9f, 0.9f, 0.9f},
        {0.001f ,0.001f, 0.001f},
        {3.0f, 3.0f, 3.0f},
    };
    std::vector<util::pos::Point3> pos_set2 = {
        {1.9f, 1.9f, 1.9f},
        {1.999f, 1.999f, 1.999f},
        {3.0f, 3.0f, 3.0f},
    };
    bbt::random::mt_random<int, 300, 600> myRand;

    GenAoiConfig();
    auto aoi = CreateAoiMap();
    int meet_count = 0;

    share::ecs::aoi::AoiSystem::GetInstance()->SetOnEnterAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){
        if (p1->GetId() != p2->GetId())
            meet_count++;
    });
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnLeaveAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){
        meet_count--;
    });

    util::pos::Point3 pos1;
    util::pos::Point3 pos2;
    for (int i = 0; i< 10000; i++) {
        auto p1 = CreateAoiObj();
        auto p2 = CreateAoiObj();
        if (pos_set1.size() >= i) {
            pos1 = pos_set1[i];
            pos2 = pos_set2[i];
        } else {
            pos1.m_x = ((float)myRand()) / 100;
            pos1.m_y = ((float)myRand()) / 100;
            pos1.m_z = ((float)myRand()) / 100;
            BOOST_ASSERT(pos1.m_x >= 3 && pos1.m_x <= 6);
            BOOST_ASSERT(pos1.m_y >= 3 && pos1.m_y <= 6);
            BOOST_ASSERT(pos1.m_z >= 3 && pos1.m_z <= 6);

            pos2.m_x = ((float)myRand()) / 100;
            pos2.m_y = ((float)myRand()) / 100;
            pos2.m_z = ((float)myRand()) / 100;
            BOOST_ASSERT(pos2.m_x >= 3 && pos2.m_x <= 6);
            BOOST_ASSERT(pos2.m_y >= 3 && pos2.m_y <= 6);
            BOOST_ASSERT(pos2.m_z >= 3 && pos2.m_z <= 6);
        }

        BOOST_ASSERT(share::ecs::aoi::AoiSystem::GetInstance()->EnterAoi(aoi, p1, pos1));
        BOOST_ASSERT(share::ecs::aoi::AoiSystem::GetInstance()->EnterAoi(aoi, p2, pos2));

        BOOST_ASSERT(share::ecs::aoi::AoiSystem::GetInstance()->LeaveAoi(aoi, p1));
        BOOST_ASSERT(share::ecs::aoi::AoiSystem::GetInstance()->LeaveAoi(aoi, p2));
    }
    printf("meet_count: %d\n", meet_count);
    BOOST_CHECK(meet_count == 0);
}

/* aoi 随机降落测试 */
BOOST_AUTO_TEST_CASE(t_aoi_enter_test)
{
    bbt::random::mt_random<int, 1, 5> rd;
    GenAoiConfig();
    int num = 10;
    auto aoi = CreateAoiMap();
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnEnterAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){});
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnLeaveAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){});

    std::vector<engine::ecs::EntitySPtr> players;
    for(int i = 1; i <= num; ++i)
    {
        auto player = CreateAoiObj();
        BOOST_CHECK(player != nullptr);
        players.push_back(player);
    }

    for(int i = 0; i< num; ++i)
    {
        share::ecs::aoi::AoiSystem::GetInstance()->EnterAoi(
            aoi,
            players[i],
            {
                (float)(rd()),
                (float)(rd()),
                (float)(rd())
            }
        );
    }
}


//TODO aoi move 接口测试

/**
 * 令地图上任意两个点，以相向的方向步进。步进速率小于灯塔视野半径，保证不会错过。
 * 
 * 接触到地图边缘停止
 * 
 * 测试原理：保证运动过程中双方都会触发一次进入和离开各自的关注列表
 */
BOOST_AUTO_TEST_CASE(t_aoi_move_test)
{
    bbt::random::mt_random<int, 200, 800> rd;   // 初始降落点范围超过1来保证会错过
    GenAoiConfig();
    auto aoi = CreateAoiMap();
    float step_rate = 0.2f;

    const int max_times = 1000;
    int count = 0;
    int meet_count = 0;

    share::ecs::aoi::AoiSystem::GetInstance()->SetOnEnterAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){
        count++;
        meet_count++;
    });
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnLeaveAoiEvent(
    [&](EntitySPtr aoi, EntitySPtr p1, EntitySPtr p2){
        count--;
    });

    for (int i = 0; i < max_times; ++i) {
        bool out_of_bound = false; // 是否有玩家出界的标志
        auto p1 = CreateAoiObj();
        auto p2 = CreateAoiObj();
        
        util::pos::Point3 pos1;
        pos1.m_x = ((float)rd()) / 100;
        pos1.m_y = ((float)rd()) / 100;
        pos1.m_z = ((float)rd()) / 100;

        util::pos::Point3 pos2;
        pos2.m_x = ((float)rd()) / 100;
        pos2.m_y = ((float)rd()) / 100;
        pos2.m_z = ((float)rd()) / 100;

        vector::Vector3 pos1_direction = vector::ChangeVectorNorm(vector::CalcVectorByPos(pos1, pos2), step_rate);
        vector::Vector3 pos2_direction = vector::ChangeVectorNorm(vector::CalcVectorByPos(pos2, pos1), step_rate);

        BOOST_CHECK(share::ecs::aoi::AoiSystem::GetInstance()->EnterAoi(aoi, p1, pos1));
        BOOST_CHECK(share::ecs::aoi::AoiSystem::GetInstance()->EnterAoi(aoi, p2, pos2));

        while (!out_of_bound) {
            pos1 = vector::MoveTo(pos1, pos1_direction);
            pos2 = vector::MoveTo(pos2, pos2_direction);

            // 相向而行
            out_of_bound |= !share::ecs::aoi::AoiSystem::GetInstance()->Move(
                aoi, p1, pos1);
            out_of_bound |= !share::ecs::aoi::AoiSystem::GetInstance()->Move(
                aoi, p1, pos2);
        }

        BOOST_CHECK(share::ecs::aoi::AoiSystem::GetInstance()->LeaveAoi(aoi, p1));
        BOOST_CHECK(share::ecs::aoi::AoiSystem::GetInstance()->LeaveAoi(aoi, p2));
    }
    
}

BOOST_AUTO_TEST_SUITE_END()