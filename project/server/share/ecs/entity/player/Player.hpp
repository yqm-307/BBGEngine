#pragma once
#include "share/ecs/GameObject.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::entity::player
{

class Player;

typedef uint64_t PlayerId;   /* 玩家id，存盘数据，不会变动 */

typedef std::shared_ptr<Player> PlayerSPtr;
typedef std::weak_ptr<Player>   PlayerWKPtr;



class Player:
    public GameObject
{
public:
    Player();
    ~Player();
    virtual void OnUpdate();
private:
};

}