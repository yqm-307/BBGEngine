#pragma once
#include <bbt/templateutil/BaseType.hpp>
#include <bbt/templateutil/Noncopyable.hpp>
#include "share/ecs/player/entity/Player.hpp"
#include "util/typedef/NamespaceType.hpp"
#include "util/hashmap/Hashmap.hpp"

namespace share::ecs::entity::playermgr
{
class PlayerMgr;
OnlyUniqueDef(PlayerMgr);

const int Default_Hash_Bucket_Num = 256;


class PlayerMgr:
    public bbt::templateutil::noncopyable,  // 无状态
    protected engine::ecs::GameObject
{
    typedef util::hashmap::Hashmap<share::ecs::entity::player::PlayerId, ecs::entity::player::PlayerSPtr, 256> PlayerMap;
public:
    typedef std::pair<share::ecs::entity::player::PlayerSPtr, bool> Result;

    static PlayerMgrCUQPtr& GetInstance();
    ~PlayerMgr();

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
    explicit PlayerMgr();
    Result KickPlayerEx(player::PlayerId id);
    virtual void OnFatherDead() override {};
private:
    PlayerMap       m_all_player;
};

};