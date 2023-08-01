#pragma once
#include <bbt/templateutil/BaseType.hpp>
#include <bbt/templateutil/Noncopyable.hpp>
#include "share/ecs/entity/player/Player.hpp"

namespace game::share::ecs::entity::playermgr
{
class PlayerMgr;

const int Default_Hash_Bucket_Num = 256;
typedef std::shared_ptr<PlayerMgr> PlayerMgrSPtr;

class PlayerMgr:
    public bbt::templateutil::noncopyable,
    public share::ecs::GameObject,
    public std::enable_shared_from_this<PlayerMgr>
{
    // typedef std::shared_ptr<>
    typedef util::hashmap::Hashmap<player::PlayerId, ecs::entity::player::PlayerSPtr, 256> PlayerMap;
public:
    typedef std::pair<player::PlayerSPtr, bool> Result;

    static PlayerMgrSPtr GetInstance();

    /**
     * @brief 根据玩家id获取在线玩家，如果玩家不在线则返回 nullptr 和 false
     * 
     * @param id 
     * @return Result 
     */
    Result GetPlayerById(player::PlayerId id);
    Result KickPlayer(player::PlayerId id);
    Result KickPlayer(player::PlayerSPtr player);
    virtual void OnUpdate() override;

private:
    PlayerMgr();
    ~PlayerMgr();
    Result KickPlayerEx(player::PlayerId id);
private:
    PlayerMap       m_all_player;
};

};