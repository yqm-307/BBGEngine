#include "share/scene/testscene/SampleScene.hpp"
#include "share/ecs/network/Network.hpp"
#include "share/ecs/network/NetworkSystem.hpp"
#include "share/session/DBServiceSession.hpp"
#include <signal.h>

int main()
{
    int dbg_terminal = 1;
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &dbg_terminal, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
    auto& SysRef = share::ecs::network::NetworkSystem::GetInstance();
    bbt::network::libevent::EventLoop eventloop;

    auto sence = share::scene::SampleScene();
    auto network_obj = G_GameObjectMgr()->Create<share::ecs::network::Network>();
    sence.MountGameObject(network_obj);

    share::ecs::network::ServerCfg cfg;
    cfg.ip = "127.0.0.1";
    cfg.port = 10011;
    cfg.connent_timeout = 1000;

    auto update_event = eventloop.CreateEvent(0, EV_PERSIST, [&](auto, auto){
        sence.Update();
    });
    auto signal_event = eventloop.CreateEvent(SIGINT, EV_SIGNAL, [&](auto, auto){
        SysRef->StopNetwork(network_obj);
        GAME_EXT1_LOG_ERROR("netowrk stop!");
    });

    Assert(SysRef->InitNetwork(network_obj, cfg));
    SysRef->StartNetwork(network_obj);
    GAME_EXT1_LOG_INFO("netowrk start!");
    // SysRef->AsyncConnect();

    eventloop.StartLoop(bbt::network::libevent::LOOP_NO_EXIT_ON_EMPTY);
}