#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/random/Random.hpp>
#include "share/ecs/aoi/entity/Aoi.hpp"
#include "share/ecs/aoi/component/AoiObjectComponent.hpp"
#include "share/ecs/none/entity/NoneObj.hpp"
#include "share/ecs/aoi/system/AoiSystem.hpp"

using namespace share;
using namespace util;
typedef share::ecs::aoi::Aoi Aoi;
typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
typedef std::shared_ptr<ecs::aoi::AoiObjectComponent> AoiObjComponentPtr;
typedef std::shared_ptr<ecs::noneobj::NoneObj> NoneObjPtr;


std::shared_ptr<ecs::aoi::AoiObjectComponent> GetAoiComponet(NoneObjPtr obj)
{
    auto aoi_obj = obj->GetComponent(ecs::emComponentType::EM_COMPONENT_TYPE_AOI_OBJECT);
    BOOST_ASSERT(aoi_obj != nullptr);
    return std::static_pointer_cast<ecs::aoi::AoiObjectComponent>(aoi_obj);
}

void AoiNotifyEvent(GameObjectSPtr aoi, GameObjectSPtr w, GameObjectSPtr m , const std::string str)
{

    auto p1 = std::static_pointer_cast<ecs::noneobj::NoneObj>(w);
    auto p2 = std::static_pointer_cast<ecs::noneobj::NoneObj>(m);
    auto pos1 = GetAoiComponet(p1);
    auto pos2 = GetAoiComponet(p2);

    printf("----------------------------------------------------\n");
    printf("[%s]对象: %d 看到 对象(%d):\n", str.c_str(), pos1->GetObjId(), pos2->GetObjId());
    // pos2->Debug_PosChange();
}

/* 创建一个纯动态的aoi对象 */
GameObjectSPtr CreateAoiObj()
{
    auto noneobj = G_GameObjectMgr()->Create<ecs::noneobj::NoneObj>();
    auto aoi_obj_component = G_ComponentMgr()->Create<ecs::aoi::AoiObjectComponent>(share::ecs::aoi::AoiEntityFlag::Watcher);
    noneobj->AddComponent(aoi_obj_component);
    return noneobj;
}

GameObjectSPtr CreateAoiMap()
{
    auto noneobj = G_GameObjectMgr()->Create<ecs::noneobj::NoneObj>();
    auto aoi_component = G_ComponentMgr()->Create<ecs::aoi::AoiComponent>();
    noneobj->AddComponent(aoi_component);
    return noneobj;
}

void GenAoiConfig()
{
    auto* aoi = new util::config::AoiConfig();
    aoi->m_map_x = 10.0f;
    aoi->m_map_y = 10.0f;
    aoi->m_map_z = 10.0f;
    aoi->m_tower_x = 1.0f;
    aoi->m_tower_y = 1.0f;
    aoi->m_tower_z = 1.0f;
    G_SetConfigPtr(util::config::AoiConfig, aoi, util::config::Cfg_Aoi);
}

BOOST_AUTO_TEST_SUITE(AoiTest)

/* 测试两个对象相遇在一个aoi中 */
BOOST_AUTO_TEST_CASE(t_aoi_2_object_meet)
{
    std::vector<util::vector::Vector3> pos_set1 = {
        {0.9f, 0.9f, 0.9f},
        {0.001f ,0.001f, 0.001f},
        {3.0f, 3.0f, 3.0f},
    };
    std::vector<util::vector::Vector3> pos_set2 = {
        {1.9f, 1.9f, 1.9f},
        {1.999f, 1.999f, 1.999f},
        {3.0f, 3.0f, 3.0f},
    };
    bbt::random::mt_random<int, 300, 600> myRand;

    GenAoiConfig();
    auto aoi = CreateAoiMap();
    int meet_count = 0;

    share::ecs::aoi::AoiSystem::GetInstance()->SetOnEnterAoiEvent(
    [&](GameObjectSPtr aoi, GameObjectSPtr p1, GameObjectSPtr p2){
        if (p1->GetId() != p2->GetId())
            meet_count++;
    });
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnLeaveAoiEvent(
    [&](GameObjectSPtr aoi, GameObjectSPtr p1, GameObjectSPtr p2){
        meet_count--;
    });

    util::vector::Vector3 pos1;
    util::vector::Vector3 pos2;
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
    [&](GameObjectSPtr aoi, GameObjectSPtr p1, GameObjectSPtr p2){});
    share::ecs::aoi::AoiSystem::GetInstance()->SetOnLeaveAoiEvent(
    [&](GameObjectSPtr aoi, GameObjectSPtr p1, GameObjectSPtr p2){});

    std::vector<engine::ecs::GameObjectSPtr> players;
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

BOOST_AUTO_TEST_SUITE_END()