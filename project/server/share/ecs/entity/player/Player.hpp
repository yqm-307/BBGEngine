#pragma once
#include "share/ecs/GameObject.hpp"
#include "share/ecs/component/aoi/AoiComponent.hpp"

namespace game::share::ecs::entity::player
{

class Player;

typedef int64_t PlayerId;   /* 玩家id，存盘数据，不会变动 */

typedef std::shared_ptr<Player> PlayerSPtr;
typedef std::weak_ptr<Player>   PlayerWKPtr;


enum OnlineStatus {
    Offline = 0,
    Online  = 1,
};


class Player:
    public GameObject
{
public:
    Player();
    ~Player();
    virtual void OnUpdate() override;
    /**
     * @brief 玩家是否在线
     * 
     * @return true 
     * @return false 
     */
    bool IsOnline();

    /**
     * @brief 获取玩家id
     * 
     * @return PlayerId 
     */
    PlayerId GetPlayerId();
private:
    PlayerId            m_player_id{-1};
    OnlineStatus        m_online_status{OnlineStatus::Online};
};

}