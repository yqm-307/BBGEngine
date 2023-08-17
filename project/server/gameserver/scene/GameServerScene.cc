#include "gameserver/scene/GameServerScene.hpp"
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

void EventSignal_Sigint(evutil_socket_t, short, void* arg)
{
    auto pthis = (GameServerScene*)arg;
    pthis->StopScene();
}

#pragma endregion

#pragma region "内部函数"



void GameServerScene::Init()
{
    /* 初始化 aoi */
    module_aoi = new game::share::ecs::entity::aoi::Aoi(
    [](GameObjectSPtr p1, GameObjectSPtr p2){
    }, 
    [](GameObjectSPtr, GameObjectSPtr){
    });

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
}

void GameServerScene::Destory()
{
    delete module_aoi;
    module_aoi = nullptr;

    event_free(m_update_event);
    event_free(m_signal_sigint);
    event_base_free(m_ev_base);

    m_ev_base = nullptr;
    m_update_event = nullptr;
    m_signal_sigint = nullptr;

}

void GameServerScene::StartScene()
{
    event_base_dispatch(m_ev_base);
}

void GameServerScene::StopScene()
{
    event_base_loopbreak(m_ev_base);
    BBT_BASE_LOG_INFO("SIGINT exist loop!");
}

#pragma endregion

} 