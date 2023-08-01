#include "share/ecs/entity/player/Player.hpp"


namespace game::share::ecs::entity::player
{
Player::Player()
    :GameObject(ecs::GameObjType::Player)
{
}


Player::~Player()
{
}

#pragma region "公共接口实现"

void Player::OnUpdate()
{
    
}

bool Player::IsOnline()
{
    return m_online_status == OnlineStatus::Online;
}

PlayerId Player::GetPlayerId()
{
    return m_player_id;
}


#pragma endregion
}