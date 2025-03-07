#include <signal.h>
#include <bbt/core/clock/Clock.hpp>
#include <bbt/core/timer/TimeWheel.hpp>
#include "engine/ecs/system/System.hpp"
#include "plugin/scene/testscene/SampleScene.hpp"
#include "plugin/ecs/network/Network.hpp"
#include "plugin/ecs/network/NetworkSystem.hpp"
#include "plugin/ecs/entity/Entity.hpp"
#include "plugin/session/DBServiceSession.hpp"
#include "plugin/ecs/timewheel/TimeWheelSystem.hpp"
#include "plugin/ecs/Define.hpp"
#include "protocol/dbservice.pb.h"

int main()
{
    int dbg_terminal = 1;
    std::shared_ptr<plugin::ecs::network::DBServiceConnection> dbconn{nullptr};
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, &dbg_terminal, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
    auto& SysRef = plugin::ecs::network::NetworkSystem::GetSysInst();
    bbt::network::libevent::EventLoop eventloop;
    plugin::session::DBServiceSession::Init();

    auto sence = plugin::scene::SampleScene();
    auto network_obj = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    sence.MountGameObject(network_obj);
    auto timer_obj = G_GameObjectMgr()->Create<plugin::ecs::gameobject::GameObject>();
    auto timer_comp = G_ComponentMgr()->Create<plugin::ecs::timewheel::TimeWheelComp>(20);
    timer_obj->AddComponent(timer_comp);
    auto& timer_system = engine::ecs::GetSystem<plugin::ecs::timewheel::TimeWheelSystem>();
    sence.MountGameObject(timer_obj);
    

    plugin::ecs::network::ServerCfg cfg;
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
    SysRef->AsyncConnect(network_obj, "192.168.1.159", 9000, 5000, [&](bbt::network::util::errcode::Errcode err, bbt::network::interface::INetConnectionSPtr conn){
        auto comp = network_obj->GetComponent(plugin::ecs::EM_COMPONENT_TYPE_CONN_MGR);
        if (comp == nullptr)
            GAME_EXT1_LOG_ERROR("network object not found connmgr!");
        auto mgr = std::static_pointer_cast<plugin::ecs::network::ConnMgr>(comp);
        Assert(mgr);


        dbconn = std::make_shared<plugin::ecs::network::DBServiceConnection>(mgr.get(), std::static_pointer_cast<bbt::network::libevent::Connection>(conn), 5000);



        bbt::core::Buffer protocol;
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
            bbt::core::Buffer protocol;
            DB_HEART_BEAT_REQ req;
            std::string bytearray;
            req.set_timestamp(bbt::core::clock::now<>().time_since_epoch().count() / 1000);
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