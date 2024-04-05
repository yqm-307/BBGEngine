#include "gameserver/scene/GameServerScene.hpp"
#include "gameserver/init/LoadConfig.hpp"
#include "share/ecs/network/NetworkSystem.hpp"
#include "share/ecs/gameobject/GameObject.hpp"
#include "share/ecs/aoi/AoiComponent.hpp"
#include "share/ecs/network/NetworkComponent.hpp"
#include "util/log/Log.hpp"
#include <signal.h>

namespace server::scene
{

GameServerScene::GameServerScene()
{
    OnCreate();
}

GameServerScene::~GameServerScene()
{
    OnDestory();
}

void GameServerScene::OnUpdate()
{
    if(m_is_stop) {
        // 释放场景内的所有游戏对象
        // auto [_aoi,isok_aoi] = UnMountGameObject(m_aoi_id);        
        // DebugAssert(isok_aoi);
        // auto [_network,isok2] = UnMountGameObject(m_network_id);
        // DebugAssert(isok2);
        
        OnStopScene();
    }
    static auto prev_time = bbt::timer::clock::now();
    if((bbt::timer::clock::now() - prev_time) >= bbt::timer::clock::ms(5000))
    {
        GAME_BASE_LOG_DEBUG("[GameServerScene::OnUpdate] is_stop=%d gameobj size=%d", m_is_stop, GetChildNum());
        prev_time = bbt::timer::clock::now();
    }
}

#pragma region "内部函数"

void GameServerScene::OnCreate()
{
    using namespace bbt::network;

    m_loop = std::make_shared<bbt::network::libevent::EventLoop>();
    {// scene update 事件注册
        m_update_event = m_loop->CreateEvent(-1, libevent::EventOpt::PERSIST, [this](auto, short events){
            this->Update();
        });

        auto err = m_update_event->StartListen(1000 / GameSceneFrame);
        if (!err)
            GAME_BASE_LOG_ERROR("[GameServerScene::OnCreate] regist scene update, %s\n", err.CWhat());
    }

    {// SIGINT 信号事件注册
        m_signal_sigint_handle = m_loop->CreateEvent(SIGINT, libevent::EventOpt::SIGNAL, [this](auto, short){
            this->StopScene();
        });
        auto err = m_signal_sigint_handle->StartListen(0);
        if (!err)
            GAME_BASE_LOG_ERROR("[GameServerScene::OnCreate] regist signal SIGNAL, %s\n", err.CWhat());
    }

    {// Aoi
        auto aoi = AoiInit();
        bool isok = MountGameObject(aoi);
        DebugAssert(isok);
        m_aoi_id = aoi->GetId();
    }

    {// network 初始化
        auto network = NetWorkInit();
        bool isok = MountGameObject(network);
        DebugAssert(isok);
        m_network_id = network->GetId();
    }
}

engine::ecs::GameObjectSPtr GameServerScene::AoiInit()
{
    /* 初始化 aoi */
    auto gameobj = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::gameobject::GameObject>();
    // auto aoi_obj = engine::ecs::GameObjectMgr::GetInstance()->Create<share::ecs::aoi::Aoi>();
    gameobj->AddComponent(G_ComponentMgr()->Create<share::ecs::aoi::AoiComponent>());

    return gameobj;
}

engine::ecs::GameObjectSPtr GameServerScene::NetWorkInit()
{
    auto& cfgInst = server::init::ServerConfig::GetInstance();
    auto ip     = cfgInst->GetServerIP();
    auto port   = cfgInst->GetServerPort();

    GAME_BASE_LOG_INFO("gameserver! IP: %s  Port: %d", ip.c_str(), port);

    auto network_obj = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    network_obj->AddComponent(G_ComponentMgr()->Create<share::ecs::network::NetworkComponent>());
    Assert(share::ecs::network::NetworkSystem::GetInstance()->InitNetwork(network_obj, ip.c_str(), port));

    return network_obj;
}

void GameServerScene::OnDestory()
{
    m_loop = nullptr;
    m_update_event = nullptr;
    m_signal_sigint_handle = nullptr;
}

void GameServerScene::StartScene()
{
    auto [obj, isok] = GetGameobjectById(m_network_id);
    DebugAssertWithInfo(isok, "can`t found netowrk object!");

    share::ecs::network::NetworkSystem::GetInstance()->StartNetwork(obj);
    auto err = m_loop->StartLoop(bbt::network::libevent::EventLoopOpt::LOOP_NO_EXIT_ON_EMPTY);
    if (!err)
        GAME_EXT1_LOG_ERROR(err.CWhat());
    
}

void GameServerScene::StopScene()
{
    if(m_is_stop)
    {
        GAME_BASE_LOG_WARN("[GameServerScene::StopScene] repeat call!");
        return;
    }

    m_is_stop = true;
}

void GameServerScene::OnStopScene()
{
    // 让io线程退出
    {
        IOThreadExit();
    }

    // 循环延时退出
    {
        auto err = m_loop->BreakLoop();
        if (!err)
            GAME_EXT1_LOG_ERROR(err.CWhat());

    }
    GAME_EXT1_LOG_INFO("[GameServerScene::OnStopScene] exit loop!");

}

void GameServerScene::IOThreadExit()
{
    auto [obj, isok] = GetGameobjectById(m_network_id);
    DebugAssert(isok);

    // auto network_ptr = std::static_pointer_cast<share::ecs::entity::network::Network>(obj);
    share::ecs::network::NetworkSystem::GetInstance()->StopNetwork(obj);
    GAME_BASE_LOG_INFO("[GameServerScene::IOThreadExit] iothread exit success!!!");
}

#pragma endregion

} 