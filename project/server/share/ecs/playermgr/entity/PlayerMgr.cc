#include "share/ecs/playermgr/entity/PlayerMgr.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::entity::playermgr
{

PlayerMgrCUQPtr& PlayerMgr::GetInstance()
{
    static std::unique_ptr<PlayerMgr> _instance{nullptr};
    if(_instance == nullptr)
    {
        /**
         * 错误写法：问题原因是对于make_unique来说，PlayerMgr构造函数不可见
         * 
         * _instance = std::make_unique<PlayerMgr>();
         */
        _instance = PlayerMgrUQPtr(new PlayerMgr()); // 临时值，默认右值引用
    }
    return _instance;
}

#pragma region "公共接口实现"

PlayerMgr::Result PlayerMgr::GetPlayerById(player::PlayerId player_id)
{
    if(player_id == 0)
        return {nullptr, false};
    
    return m_all_player.Find(player_id);
}

PlayerMgr::Result PlayerMgr::KickPlayer(player::PlayerId id)
{
    return KickPlayerEx(id);
}

PlayerMgr::Result PlayerMgr::KickPlayer(player::PlayerSPtr player)
{
    player::PlayerSPtr ret = nullptr;
    bool success = false;
    do
    {
        if(player == nullptr)
            break;

        auto id = player->GetPlayerId();
        if(id < 0)
            break;

        auto [old_player, isok] = KickPlayerEx(id);
        if(!isok)
            break;
        
        DebugAssert(old_player == player);
        ret = old_player;
        success = true;
    }while(0);

    return {ret, success};
}

void PlayerMgr::OnUpdate()
{

}

#pragma endregion



#pragma region "内部接口实现"

PlayerMgr::PlayerMgr()
    :engine::ecs::GameObject(share::ecs::EM_ENTITY_TYPE_PLAYER),
    m_all_player([](const player::PlayerId& key){ return key % Default_Hash_Bucket_Num; }, nullptr)
{
}

PlayerMgr::~PlayerMgr()
{
}

PlayerMgr::Result PlayerMgr::KickPlayerEx(player::PlayerId id)
{
    player::PlayerSPtr ret = nullptr;
    bool success = false;

    do
    {
        if(id < 0)
            break;

        auto [player, isexist] = m_all_player.Earse(id);
        if(!isexist)
            break;
        
        if(player == nullptr)
            break;

        if(!player->IsOnline())
            break;

        ret = player;
        success = true;
    }while(0);
    
    return {ret, success};
}

#pragma endregion

}