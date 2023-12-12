#define BOOST_TEST_INCLUDED
#include <boost/test/included/unit_test.hpp>
#include "share/ecs/aoi/entity/Aoi.hpp"
#include "share/ecs/player/entity/Player.hpp"
#include <vector>
#include <bbt/random/Random.hpp>

using namespace share;
using namespace util;
typedef share::ecs::entity::aoi::Aoi Aoi;

void print_notify(engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m , const std::string str)
{

    auto p1 = std::static_pointer_cast<ecs::entity::player::Player>(w);
    auto p2 = std::static_pointer_cast<ecs::entity::player::Player>(m);
    auto pos1 = Aoi::GetAoiComponent(p1);
    auto pos2 = Aoi::GetAoiComponent(p2);
    // printf("----------------------------------------------------\n");
    // printf("[%s]对象: %d 看到 对象(%d):\n", str.c_str(), pos1->GetObjId(), pos2->GetObjId());
    // pos2->Debug_PosChange();
}

engine::ecs::GameObjectSPtr create_player(int aoi_id)
{
    auto player = engine::ecs::GameObjectMgr::GetInstance()->Create<ecs::entity::player::Player>();
    // auto player = std::make_shared<ecs::entity::player::Player>();
    auto aoi_comp = G_ComponentMgr()->Create<ecs::component::AoiComponent>();
    player->AddComponent(aoi_comp);
    aoi_comp->SetObjId(aoi_id);
    return player;
}

void aoiconfig()
{
    auto* aoi = new util::config::AoiConfig();
    aoi->m_map_x = 100.0f;
    aoi->m_map_y = 100.0f;
    aoi->m_map_z = 100.0f;
    aoi->m_tower_x = 3.0f;
    aoi->m_tower_y = 3.0f;
    aoi->m_tower_z = 3.0f;
    G_SetConfigPtr(util::config::AoiConfig, aoi, util::config::Cfg_Aoi);
}

BOOST_AUTO_TEST_SUITE(AoiTest)

/* aoi 随机降落测试 */
BOOST_AUTO_TEST_CASE(t_aoi_enter_test)
{
    bbt::random::mt_random<int, 1, 12> rd;
    aoiconfig();
    int num = 10;
    auto aoi = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::entity::aoi::Aoi>(
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        print_notify(w, m, "进入通知");
    },
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        print_notify(w, m, "离开通知");
    }
    );

    std::vector<engine::ecs::GameObjectSPtr> players;
    for(int i = 1; i <= num; ++i)
    {
        auto player = create_player(i);
        BOOST_CHECK(player != nullptr);
        players.push_back(player);
    }

    for(int i = 0; i< num; ++i)
    {
        aoi->EnterAoi(
            players[i],
            {
                (float)(40 + rd()),
                (float)(40 + rd()),
                (float)(40 + rd())
            }
        );
    }
}

/* aoi z轴为0，地面移动测试 */
BOOST_AUTO_TEST_CASE(t_aoi_move_test)
{
    system("clear");
    // int num = 10;
    auto aoi = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::entity::aoi::Aoi>(
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        print_notify(w, m, "进入通知");
    },
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        print_notify(w, m, "离开通知");
    }
    );

    auto p1 = create_player(1);
    auto p2 = create_player(2);
    
    aoi->EnterAoi(
        p1,
        {
            10,
            10,
            0
        }
    );
    aoi->EnterAoi(
        p2,
        {
            10,
            20,
            0
        }
    );

    for(int i = 0; i<10; ++i)
    {
        aoi->Move(p1,
            {
                10,
                (float)(10 + i),
                0
            }
        );
    }

}


// 对 aoi 接口进行测试
BOOST_AUTO_TEST_CASE(t_aoi_api_test)
{
    // const int num = 100;
    bbt::random::mt_random<int, 0, 100> rd;
    auto aoi = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::entity::aoi::Aoi>(
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        // print_notify(w, m, "进入通知");
    },
    [](engine::ecs::GameObjectSPtr w, engine::ecs::GameObjectSPtr m){
        // print_notify(w, m, "离开通知");
    }
    );

    auto p1 = create_player(1);
    auto p2 = create_player(2);
    
    std::vector<engine::ecs::GameObjectSPtr> players;
    /* 每个灯塔放一个 */
    for(int i = 1; i <= 34*34*34; ++i)
    {
        auto player = create_player(i);
        BOOST_CHECK(player != nullptr);
        players.push_back(player);
    }

    /* 每个灯塔放一个 */
    for(int i = 0; i < 34; ++i) {
        for(int j = 0; j < 34; ++j) {
            for(int k = 0; k < 34; ++k) {
                aoi->EnterAoi(players[i*34*34 + j*34 + k], {(float)i * 3, (float)j * 3, (float)k * 3} );
            }
        }
    }
    std::map<int, int> stat;

    for(auto&& obj : players)
    {
        auto vec = aoi->GetEntitysByGameobj(obj);
        BOOST_CHECK(!vec.empty());
        stat[vec.size()]++;        
    }

}


BOOST_AUTO_TEST_SUITE_END()