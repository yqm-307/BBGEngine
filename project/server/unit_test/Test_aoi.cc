#define BOOST_TEST_MODULE aoi test
#include <boost/test/included/unit_test.hpp>
#include "share/ecs/entity/aoi/Aoi.hpp"
#include "share/ecs/entity/player/Player.hpp"
#include <vector>
#include <bbt/random/Random.hpp>

using namespace game::share;
using namespace game::util;
typedef game::share::ecs::entity::aoi::Aoi Aoi;

void print_notify(game::share::ecs::GameObject::SPtr w, game::share::ecs::GameObject::SPtr m)
{

    auto p1 = std::static_pointer_cast<ecs::entity::Player>(w);
    auto p2 = std::static_pointer_cast<ecs::entity::Player>(m);
    auto pos1 = Aoi::GetAoiComponent(p1);
    auto pos2 = Aoi::GetAoiComponent(p2);
    printf("\n----------------------------------------------------\n");
    printf("对象: %d 看到 对象(%d):\n", pos1->GetObjId(), pos2->GetObjId());
    pos2->Debug_PosChange();
}

game::share::ecs::GameObject::SPtr create_player(int aoi_id)
{
    auto player = std::make_shared<ecs::entity::Player>();
    auto aoi_comp = std::make_shared<ecs::component::AoiComponent>();
    player->AddComponent(aoi_comp);
    aoi_comp->SetObjId(aoi_id);
    return player;
}

void aoiconfig()
{
    auto* aoi = new game::util::config::AoiConfig();
    aoi->m_map_x = 100.0f;
    aoi->m_map_y = 100.0f;
    aoi->m_map_z = 100.0f;
    aoi->m_tower_x = 3.0f;
    aoi->m_tower_y = 3.0f;
    aoi->m_tower_z = 3.0f;
    G_SetConfigPtr(game::util::config::AoiConfig, aoi, game::util::config::Cfg_Aoi);
}

/* aoi 随机移动测试 */
BOOST_AUTO_TEST_CASE(t_aoi_base_test)
{
    bbt::random::mt_random<int, 1, 12> rd;
    aoiconfig();
    int num = 10;
    auto aoi = game::share::ecs::entity::aoi::Aoi::Create(
    [](game::share::ecs::GameObject::SPtr w, game::share::ecs::GameObject::SPtr m){
        print_notify(w, m);
    },
    [](game::share::ecs::GameObject::SPtr w, game::share::ecs::GameObject::SPtr m){
        print_notify(w, m);
    }
    );

    std::vector<game::share::ecs::GameObject::SPtr> players;
    for(int i = 1; i <= num; ++i)
    {
        auto player = create_player(i);
        BOOST_ASSERT(player != nullptr);
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