#include <signal.h>
#include "loginserver/confighelp/LoadConfig.hpp"
#include "share/ecs/network/NetworkSystem.hpp"
#include "share/ecs/gameobject/GameObject.hpp"
#include "share/ecs/aoi/AoiComponent.hpp"
#include "share/ecs/network/NetworkComponent.hpp"
#include "share/ecs/network/DBServiceConnObj.hpp"
#include "share/ecs/network/DBServiceCliComp.hpp"
#include "share/ecs/network/Network.hpp"
#include "share/ecs/globalmgr/GlobalMgr.hpp"
#include "share/scene/SceneDefine.hpp"
#include "util/log/Log.hpp"
#include "LoginServerScene.hpp"

namespace login::scene
{

LoginServerScene::LoginServerScene()
{
}

LoginServerScene::~LoginServerScene()
{
}

void LoginServerScene::OnUpdate()
{
    if(m_is_stop) {
        OnStopScene();
    }
    static auto prev_time = bbt::timer::clock::now();
    if((bbt::timer::clock::now() - prev_time) >= bbt::timer::clock::ms(5000))
    {
        GAME_EXT1_LOG_DEBUG("is_stop=%d gameobj size=%d", m_is_stop, GetChildNum());
        prev_time = bbt::timer::clock::now();
    }
}

void LoginServerScene::Init()
{
    share::scene::g_scene = std::unique_ptr<engine::scene::Scene>(this);
    OnInit();
}

void LoginServerScene::OnInit()
{
    using namespace bbt::network;
    m_loop = std::make_shared<bbt::network::libevent::EventLoop>();

    {// scene update 事件注册
        m_update_event = m_loop->CreateEvent(-1, libevent::EventOpt::PERSIST, [this](auto, short events){
            this->Update();
        });

        auto err = m_update_event->StartListen(20);
        if (!err)
            GAME_EXT1_LOG_ERROR("regist scene update, %s\n", err.CWhat());
    }

    {// SIGINT 信号事件注册
        m_signal_sigint_handle = m_loop->CreateEvent(SIGINT, libevent::EventOpt::SIGNAL, [this](auto, short){
            this->StopScene();
        });
        auto err = m_signal_sigint_handle->StartListen(0);
        if (!err)
            GAME_EXT1_LOG_ERROR("regist signal SIGNAL, %s\n", err.CWhat());
    }

    {// global mgr 注册
        auto globalmgr = GlobalMgrInit();
        bool isok = MountGameObject(globalmgr);
        DebugAssert(isok);
    }

    {// network 初始化
        auto network = NetWorkInit();
        bool isok = MountGameObject(network);
        DebugAssert(isok);
        isok = share::scene::RegistGlobalInst(network);
        DebugAssertWithInfo(isok, "gameobject is repeat in global mgr!");
        m_network_id = network->GetId();
    }

    {// db service init
        auto dbservice_cli = DBServiceInit();
        bool isok = MountGameObject(dbservice_cli);
        DebugAssert(isok);
    }
}

void LoginServerScene::StartScene()
{
    auto [obj, isok] = GetGameobjectById(m_network_id);
    DebugAssertWithInfo(isok, "can`t found netowrk object!");

    share::ecs::network::NetworkSystem::GetSysInst()->StartNetwork(obj);
    auto err = m_loop->StartLoop(bbt::network::libevent::EventLoopOpt::LOOP_NO_EXIT_ON_EMPTY);
    if (!err)
        GAME_EXT1_LOG_ERROR(err.CWhat());   
}

void LoginServerScene::StopScene()
{
    if(m_is_stop) {
        GAME_BASE_LOG_WARN("[LoginServerScene::StopScene] repeat call!");
        return;
    }

    m_is_stop = true;
}

engine::ecs::GameObjectSPtr LoginServerScene::GlobalMgrInit()
{
    auto global_mgr = G_GameObjectMgr()->Create<share::ecs::globalmgr::GlobalMgr>();
    return global_mgr;
}

engine::ecs::GameObjectSPtr LoginServerScene::NetWorkInit()
{
    auto& cfgInst = server::init::ServerConfig::GetInstance();
    auto ip     = cfgInst->GetServerIP();
    auto port   = cfgInst->GetServerPort();

    GAME_BASE_LOG_INFO("gameserver! IP: %s  Port: %d", ip.c_str(), port);

    share::ecs::network::ServerCfg cfg;
    cfg.ip = ip;
    cfg.port = port;
    cfg.connent_timeout = 1000;

    auto network_obj = G_GameObjectMgr()->Create<share::ecs::network::Network>();
    share::ecs::network::NetworkSystem::GetSysInst()->InitNetwork(network_obj, cfg);

    return network_obj;
}

engine::ecs::GameObjectSPtr LoginServerScene::DBServiceInit()
{
    auto& g_config = server::init::ServerConfig::GetInstance();
    auto dbservice_obj = G_GameObjectMgr()->Create<share::ecs::network::DBServiceConnObj>();
    auto& system_ref = engine::ecs::GetSystem<share::ecs::network::DBServiceCliCompSystem>();
    share::ecs::network::DBServiceCliCfg cfg;
    auto g_db_service_cfg = g_config->GetDBServiceCfg();
    cfg.ip              = g_db_service_cfg->ip;
    cfg.port            = g_db_service_cfg->port;
    cfg.timeout         = g_db_service_cfg->timeout;
    cfg.heartbeat       = g_db_service_cfg->heartbeat;
    cfg.connect_timeout = g_db_service_cfg->connect_timeout;

    system_ref->Init(dbservice_obj, &cfg);

    return dbservice_obj;
}

void LoginServerScene::IOThreadExit()
{
    auto [obj, isok] = GetGameobjectById(m_network_id);
    DebugAssert(isok);

    // auto network_ptr = std::static_pointer_cast<share::ecs::entity::network::Network>(obj);
    share::ecs::network::NetworkSystem::GetSysInst()->StopNetwork(obj);
    GAME_BASE_LOG_INFO("[GameServerScene::IOThreadExit] iothread exit success!!!");
}

void LoginServerScene::OnStopScene()
{
    // 让io线程退出
    {
        IOThreadExit();
    }

    {
        auto err = m_loop->BreakLoop();
        if (!err)
            GAME_EXT1_LOG_ERROR(err.CWhat());
    }
    GAME_EXT1_LOG_INFO("[GameServerScene::OnStopScene] exit loop!");

}

} // namespace login::scene
