#pragma once
#include <bbt/pollevent/EventLoop.hpp>
#include <engine/scene/Scene.hpp>


namespace login::scene
{

class LoginServerScene:
    public engine::scene::Scene
{
public:
    LoginServerScene();
    virtual ~LoginServerScene();

    void Init();
    void StartScene();
    void StopScene();
protected:
    virtual void OnUpdate() override;
    void OnInit();
    engine::ecs::GameObjectSPtr GlobalMgrInit();
    engine::ecs::GameObjectSPtr NetWorkInit();
    engine::ecs::GameObjectSPtr DBServiceInit();
    void IOThreadExit();
    void OnStopScene();
private:
    std::shared_ptr<bbt::pollevent::EventLoop>              m_loop{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_update_event;
    std::shared_ptr<bbt::pollevent::Event>                  m_signal_sigint_handle;
    volatile bool                                           m_is_stop{false};          // 是否停止            

    engine::ecs::GameObjectId                               m_network_id;
};

}