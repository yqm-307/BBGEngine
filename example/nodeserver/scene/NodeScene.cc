#include <signal.h>
#include <nodeserver/scene/NodeScene.hpp>
#include <nodeserver/config/LoadConfig.hpp>
#include <nodeserver/ecs/network/EchoServer.hpp>
#include <plugin/ecs/network/ServerSystem.hpp>
#include <plugin/ecs/gameobject/GameObject.hpp>
#include <plugin/ecs/aoi/AoiComponent.hpp>
#include <plugin/ecs/globalmgr/GlobalMgr.hpp>
#include <plugin/scene/SceneDefine.hpp>
#include <util/log/Log.hpp>

namespace BBTENGINE_NODE_NAMESPACE::scene
{

NodeScene::NodeScene():
    m_main_scene(std::make_shared<engine::ecs::Scene>())
{
    m_main_scene->Init();
}

NodeScene::~NodeScene()
{
    OnDestory();
}

void NodeScene::OnUpdate()
{
    if(m_is_stop) {
        // 释放场景内的所有游戏对象
        // auto [_aoi,isok_aoi] = UnMountGameObject(m_aoi_id);        
        // DebugAssert(isok_aoi);
        // auto [_network,isok2] = UnMountGameObject(m_network_id);
        // DebugAssert(isok2);
        
        OnStopScene();
    }

    m_main_scene->Update();

    static auto prev_time = bbt::timer::clock::now();
    if((bbt::timer::clock::now() - prev_time) >= bbt::timer::clock::ms(5000))
    {
        GAME_BASE_LOG_DEBUG("[GameServerScene::OnUpdate] is_stop=%d gameobj size=%ld", m_is_stop, m_main_scene->GetGameObjectMgr()->ObjCount());
        prev_time = bbt::timer::clock::now();
    }
}

void NodeScene::Init()
{
    // plugin::scene::MainScene() = std::unique_ptr<engine::scene::Scene>(this);
    OnInit();
}

#pragma region "内部函数"

void NodeScene::OnInit()
{
    using namespace bbt::network;

    m_loop = std::make_shared<bbt::pollevent::EventLoop>();

    {// scene update 事件注册
        m_update_event = m_loop->CreateEvent(-1, bbt::pollevent::EventOpt::PERSIST, [this](auto, short events){
            this->OnUpdate();
        });

        auto err = m_update_event->StartListen(1000 / GameSceneFrame);
        if (err != 0)
            GAME_BASE_LOG_ERROR("[GameServerScene::OnCreate] regist scene update failed!");
    }

    {// SIGINT 信号事件注册
        m_signal_sigint_handle = m_loop->CreateEvent(SIGINT, bbt::pollevent::EventOpt::SIGNAL, [this](auto, short){
            this->StopScene();
        });

        int err = m_signal_sigint_handle->StartListen(0);
        if (err != 0)
            GAME_BASE_LOG_ERROR("[GameServerScene::OnCreate] regist signal SIGNAL failed!");
    }

    {// global mgr 注册
        // auto globalmgr = GlobalMgrInit();
        // bool isok = MountGameObject(globalmgr);
        // DebugAssert(isok);
    }

    {// network 初始化
        // auto network = NetWorkInit();
        // bool isok = MountGameObject(network);
        // DebugAssert(isok);
        // isok = plugin::scene::RegistGlobalInst(network);
        // DebugAssertWithInfo(isok, "gameobject is repeat in global mgr!");
        // m_network_id = network->GetId();
    }

}

engine::ecs::GameObjectSPtr NodeScene::GlobalMgrInit()
{
    // auto global_mgr = G_GameObjectMgr()->Create<plugin::ecs::globalmgr::GlobalMgr>();
    // return global_mgr;
    return nullptr;
}

engine::ecs::GameObjectSPtr NodeScene::NetWorkInit()
{
    auto& cfgInst = BBTENGINE_NODE_NAMESPACE::config::ServerConfig::GetInstance();
    auto ip     = cfgInst->GetServerIP();
    auto port   = cfgInst->GetServerPort();

    GAME_BASE_LOG_INFO("gameserver! IP: %s  Port: %d", ip.c_str(), port);

    // auto network_obj = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    // network_obj->AddComponent<network::EchoService>(ip, port, 5000);

    return nullptr;
}

void NodeScene::OnDestory()
{
    m_loop = nullptr;
    m_update_event = nullptr;
    m_signal_sigint_handle = nullptr;
}

void NodeScene::StartScene()
{
    // auto [obj, isok] = GetGameobjectById(m_network_id);
    // DebugAssertWithInfo(isok, "can`t found netowrk object!");

    // // plugin::ecs::network::ServerSystem<network::EchoService>::GetSysInst()->StartNetwork(obj);
    // auto err = m_loop->StartLoop(bbt::pollevent::EventLoopOpt::LOOP_NO_EXIT_ON_EMPTY);
    // if (err != 0)
    //     GAME_EXT1_LOG_ERROR("[GameServerScene::StartScene] eventloop start failed!");   
}

void NodeScene::StopScene()
{
    if(m_is_stop)
    {
        GAME_BASE_LOG_WARN("[GameServerScene::StopScene] repeat call!");
        return;
    }

    m_is_stop = true;
}

void NodeScene::OnStopScene()
{
    // 让io线程退出
    {
        IOThreadExit();
    }

    // 循环延时退出
    {
        auto err = m_loop->BreakLoop();
        if (err != 0)
            GAME_EXT1_LOG_ERROR("[GameServerScene::OnStopScene] eventloop break failed!");

    }
    GAME_EXT1_LOG_INFO("[GameServerScene::OnStopScene] exit loop!");

}

void NodeScene::IOThreadExit()
{
    // auto [obj, isok] = GetGameobjectById(m_network_id);
    // DebugAssert(isok);

    // plugin::ecs::network::ServerSystem<network::EchoService>::GetSysInst()->StopNetwork(obj);
    // GAME_BASE_LOG_INFO("[GameServerScene::IOThreadExit] iothread exit success!!!");
}

#pragma endregion

} 