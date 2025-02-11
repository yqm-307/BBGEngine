#pragma once
#include <bbt/pollevent/EventLoop.hpp>
#include <bbt/pollevent/Event.hpp>
#include "engine/scene/Scene.hpp"
#include "nodeserver/NodeDefine.hpp"

/**
 * @brief node server scene
 * 节点服场景
 * 
 */
namespace BBTENGINE_NODE_NAMESPACE::scene
{

const int GameSceneFrame = 20;  // 服务端游戏场景每秒20帧

class ServerScene:
    public engine::scene::Scene
{
    typedef engine::ecs::GameObjectSPtr    GameObjectSPtr;
public:
    ServerScene();
    ~ServerScene();

    void Init();
    /* 启动场景，开启游戏 */
    void StartScene();
    /* TODO 优雅退出 关闭场景 */
    void StopScene();
private:
    virtual void OnUpdate() override;
    void OnInit();
    void OnDestory();
private:
    /**
     * Module 流程控制
     */
    engine::ecs::GameObjectSPtr GlobalMgrInit();
    engine::ecs::GameObjectSPtr NetWorkInit();
    /* 阻塞的等待IO线程退出 */
    void IOThreadExit();
    void OnStopScene();
private:
    std::shared_ptr<bbt::pollevent::EventLoop>              m_loop{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_update_event{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_signal_sigint_handle{nullptr};
    volatile bool                                           m_is_stop{false};          // 是否停止            

    engine::ecs::GameObjectId                               m_network_id;
};

} // namespace end