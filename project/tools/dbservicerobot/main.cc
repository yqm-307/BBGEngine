#include <signal.h>
#include <bbt/base/clock/Clock.hpp>
#include <bbt/base/timer/TimeWheel.hpp>
#include "engine/ecs/system/System.hpp"
#include "share/scene/testscene/SampleScene.hpp"
#include "share/ecs/network/Network.hpp"
#include "share/ecs/network/NetworkSystem.hpp"
#include "share/ecs/gameobject/GameObject.hpp"
#include "share/session/DBServiceSession.hpp"
#include "share/ecs/timewheel/TimeWheelSystem.hpp"
#include "share/ecs/Define.hpp"
#include "protocol/dbservice.pb.h"

int main()
{
    int dbg_terminal = 1;
    std::shared_ptr<share::ecs::network::DBServiceConnection> dbconn{nullptr};
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &dbg_terminal, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
    auto& SysRef = share::ecs::network::NetworkSystem::GetInstance();
    bbt::network::libevent::EventLoop eventloop;
    share::session::DBServiceSession::Init();

    auto sence = share::scene::SampleScene();
    auto network_obj = G_GameObjectMgr()->Create<share::ecs::network::Network>();
    sence.MountGameObject(network_obj);
    auto timer_obj = G_GameObjectMgr()->Create<share::ecs::gameobject::GameObject>();
    auto timer_comp = G_ComponentMgr()->Create<share::ecs::timewheel::TimeWheelComp>(20);
    timer_obj->AddComponent(timer_comp);
    auto& timer_system = engine::ecs::GetSystem<share::ecs::timewheel::TimeWheelSystem>();
    sence.MountGameObject(timer_obj);
    

    share::ecs::network::ServerCfg cfg;
    cfg.ip = "127.0.0.1";
    cfg.port = 10011;
    cfg.connent_timeout = 5000;

    auto update_event = eventloop.CreateEvent(0, EV_PERSIST, [&](auto, auto){
        sence.Update();
    });
    update_event->StartListen(50);
    auto signal_event = eventloop.CreateEvent(SIGINT, EV_SIGNAL, [&](auto, auto){
        SysRef->StopNetwork(network_obj);
        GAME_EXT1_LOG_ERROR("netowrk stop!");
    });

    Assert(SysRef->InitNetwork(network_obj, cfg));
    SysRef->StartNetwork(network_obj);
    GAME_EXT1_LOG_INFO("netowrk start!");
    SysRef->AsyncConnect(network_obj, "192.168.1.159", 9000, 5000, [&](bbt::network::Errcode err, bbt::network::interface::INetConnectionSPtr conn){
        auto comp = network_obj->GetComponent(share::ecs::EM_COMPONENT_TYPE_CONN_MGR);
        if (comp == nullptr)
            GAME_EXT1_LOG_ERROR("network object not found connmgr!");
        auto mgr = std::static_pointer_cast<share::ecs::network::ConnMgr>(comp);
        Assert(mgr);


        dbconn = std::make_shared<share::ecs::network::DBServiceConnection>(mgr.get(), std::static_pointer_cast<bbt::network::libevent::Connection>(conn), 5000);



        bbt::buffer::Buffer protocol;
        DB_PING_REQ req;
        std::string bytearray;
        req.set_timestamp(bbt::timer::clock::now().time_since_epoch().count() / 1000);

        bytearray = req.SerializeAsString();
        int len = bytearray.size() + sizeof(int32_t) * 2;
        protocol.WriteInt32(bytearray.size() + sizeof(int32_t) * 2);
        protocol.WriteInt32(0x1);
        protocol.WriteString(bytearray);
        dbconn->Send(protocol.Peek(), protocol.DataSize());

        // printf("len: %d", len);
        timer_system->AddTask(timer_obj, [dbconn](){
            bbt::buffer::Buffer protocol;
            DB_HEART_BEAT_REQ req;
            std::string bytearray;
            req.set_timestamp(bbt::clock::now<>().time_since_epoch().count() / 1000);
            bytearray = req.SerializeAsString();
            protocol.WriteInt32(bytearray.size() + sizeof(int32_t) * 2);
            protocol.WriteInt32(0x3);
            protocol.WriteString(bytearray);
        
            dbconn->Send(protocol.Peek(), protocol.DataSize());
            return true;
        }, 20 * 3);
        GAME_BASE_LOG_DEBUG("len: %d", len);
    });

    eventloop.StartLoop(bbt::network::libevent::LOOP_NO_EXIT_ON_EMPTY);
}