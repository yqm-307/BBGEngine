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

void Player::OnUpdate()
{
    
}


}