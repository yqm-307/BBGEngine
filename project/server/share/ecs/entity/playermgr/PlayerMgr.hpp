#pragma once
#include <bbt/templateutil/BaseType.hpp>
#include <bbt/templateutil/Noncopyable.hpp>
#include "share/ecs/entity/player/Player.hpp"

namespace game::share::ecs::entity::playermgr
{

const int Default_Hash_Bucket_Num = 256;

class PlayerMgr:
    public bbt::templateutil::BaseType<PlayerMgr>,
    public bbt::templateutil::noncopyable
{
    // typedef std::shared_ptr<>
    typedef util::hashmap::Hashmap<player::PlayerId, ecs::entity::player::PlayerSPtr, 256> PlayerMap;
public:
    static SPtr GetInstance();

    player::PlayerSPtr GetPlayerById(player::PlayerId);
private:
    PlayerMgr();
private:
    PlayerMap       m_all_player;
};

};