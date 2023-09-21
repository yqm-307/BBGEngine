#include "gameserver/scene/GameServerScene.hpp"
#include "gameserver/init/LoadConfig.hpp"
#include "gameserver/network/Network.hpp"
#include "util/log/Log.hpp"
#include <signal.h>

namespace server::scene
{

GameServerScene::GameServerScene()
{
    Init();
}

GameServerScene::~GameServerScene()
{
    Destory();
}

void GameServerScene::OnUpdate()
{
    DebugAssert(module_aoi != nullptr);
    module_aoi->OnUpdate();  // 更新aoi
}

#pragma region "事件函数"

void EventUpdate(evutil_socket_t,short,void* arg)
{
    auto pthis = (GameServerScene*)arg;
    pthis->OnUpdate();
}

// SIGINT 信号
void EventSignal_Sigint(evutil_socket_t fd, short events, void* arg)
{
    GAME_BASE_LOG_WARN("signal handle recv SIGINT , events=%d", events);
    auto pthis = (GameServerScene*)arg;
    pthis->StopScene();
}

#pragma endregion

#pragma region "内部函数"



void GameServerScene::Init()
{


    m_ev_base = event_base_new();
    {// scene update 事件注册
        timeval tv;
        evutil_timerclear(&tv);
        tv.tv_usec = GameSceneFrame * 1000;
        /* 随意的暴露this，这不是一个很好的做法。todo：寻求更好的兼容C风格Api的方式 */
        m_update_event = event_new(m_ev_base, -1, EV_PERSIST, EventUpdate, this);
        Assert(m_update_event != nullptr);
        Assert(event_add(m_update_event, &tv) == 0);
    }

    {// SIGINT 信号事件注册
        m_signal_sigint = evsignal_new(m_ev_base, SIGINT, EventSignal_Sigint, this);
        Assert(m_signal_sigint != nullptr);
        Assert(event_add(m_signal_sigint, NULL) == 0);
    }

    {
        AoiInit();
    }

    {// network 初始化
        NetWorkInit();
    }
}

void GameServerScene::AoiInit()
{
    /* 初始化 aoi */
    module_aoi = new share::ecs::entity::aoi::Aoi(
    [](GameObjectSPtr p1, GameObjectSPtr p2){
    }, 
    [](GameObjectSPtr, GameObjectSPtr){
    });
}

void GameServerScene::AoiDestory()
{
    delete module_aoi;
    module_aoi = nullptr;
}

void GameServerScene::NetWorkInit()
{
    auto& cfgInst = server::init::ServerConfig::GetInstance();
    auto ip     = cfgInst->GetServerIP();
    auto port   = cfgInst->GetServerPort();

    GAME_BASE_LOG_INFO("World Server! IP: %s  Port: %d", ip.c_str(), port);

    module_network = new server::network::Network(ip, port);
}

void GameServerScene::NetWorkDestory()
{
    module_network->SyncStop();

    delete module_network;
    module_network = nullptr;
}

void GameServerScene::Destory()
{
    AoiDestory();
    NetWorkDestory();

    event_free(m_update_event);
    event_free(m_signal_sigint);
    event_base_free(m_ev_base);

    m_ev_base = nullptr;
    m_update_event = nullptr;
    m_signal_sigint = nullptr;

}

void GameServerScene::StartScene()
{
    module_network->SyncStart();
    event_base_dispatch(m_ev_base);
}

void GameServerScene::StopScene()
{
    event_base_loopbreak(m_ev_base);
    BBT_BASE_LOG_INFO("SIGINT exist loop!");
}

#pragma endregion

} 