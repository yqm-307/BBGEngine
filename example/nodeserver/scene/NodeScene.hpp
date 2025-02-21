#pragma once
#include <bbt/pollevent/EventLoop.hpp>
#include <bbt/pollevent/Event.hpp>
#include <engine/ecs/entity/Entity.hpp>
#include <engine/ecs/scene/Scene.hpp>
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

class NodeScene
{
    typedef engine::ecs::EntitySPtr    EntitySPtr;
public:
    NodeScene();
    ~NodeScene();

    void Init();
    /* 启动场景，开启游戏 */
    void StartScene();
    /* TODO 优雅退出 关闭场景 */
    void StopScene();
private:
    void OnUpdate();
    void OnInit();
    void OnDestory();
private:
    /**
     * Module 流程控制
     */
    engine::ecs::EntitySPtr GlobalMgrInit();
    engine::ecs::EntitySPtr NetWorkInit();
    /* 阻塞的等待IO线程退出 */
    void IOThreadExit();
    void OnStopScene();
private:
    engine::ecs::SceneSPtr                                  m_main_scene{nullptr};    
    std::shared_ptr<bbt::pollevent::EventLoop>              m_loop{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_update_event{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_signal_sigint_handle{nullptr};
    volatile bool                                           m_is_stop{false};          // 是否停止            

    engine::ecs::GameObjectId                               m_network_id;
};

} // namespace end