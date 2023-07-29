#include "system/testsystem.hpp"
#include "share/ecs/entity/aoi/Aoi.hpp"

int main()
{
    SetConfig();
    auto* aoi = G_GetConfigPtr(game::util::config::AoiConfig, game::util::config::Cfg_Aoi);
    printf("%0.f %0.f %0.f %0.f %0.f %0.f \n", aoi->m_map_x, aoi->m_map_y, aoi->m_map_z,
        aoi->m_tower_x, aoi->m_tower_y, aoi->m_tower_z);
    game::share::ecs::entity::Aoi myaoi(
        [](game::share::ecs::GameObject::SPtr, game::share::ecs::GameObject::SPtr){},
        [](game::share::ecs::GameObject::SPtr, game::share::ecs::GameObject::SPtr){}
    );
}