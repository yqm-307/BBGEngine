#include "Player.hpp"


namespace game::share::ecs::entity
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