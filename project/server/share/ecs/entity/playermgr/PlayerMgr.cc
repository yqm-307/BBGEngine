#include "share/ecs/entity/playermgr/PlayerMgr.hpp"

namespace game::share::ecs::entity::playermgr
{

PlayerMgr::SPtr PlayerMgr::GetInstance()
{
    static SPtr instance = nullptr;
    if(instance == nullptr)
    {
        instance = std::make_shared<PlayerMgr>();
    }
    return instance;
}

#pragma region "公共接口实现"

player::PlayerSPtr PlayerMgr::GetPlayerById(player::PlayerId player_id)
{
    if(player_id == 0)
        return nullptr;
    
    auto [player, isexist] = m_all_player.Find(player_id);
    return player;
}

#pragma endregion



#pragma region "内部接口实现"

PlayerMgr::PlayerMgr()
    :m_all_player([](const player::PlayerId& key){ return key%Default_Hash_Bucket_Num; }, nullptr)
{
}


#pragma endregion

}