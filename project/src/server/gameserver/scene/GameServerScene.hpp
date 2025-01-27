#pragma once
#include <bbt/pollevent/EventLoop.hpp>
#include <bbt/pollevent/Event.hpp>
#include "engine/scene/Scene.hpp"

/**
 * 关于游戏场景的理解
 * 
 * 在客户端，场景不是一成不变的。那么场景为啥会发生变化？
 * 
 * 因为有驱动力，有行为驱动场景的变化。
 * 
 * 在我的理解中Unity中驱动场景变化的方式就两种：内驱动、外驱动。对于两个名词我的理解如下：
 *  1、自驱动。是指Scene本身在驱动自身发生变化（Update系列函数以及其产生的影响）
 *  2、外驱动。是指外部行为导致Scene本身发生变化（服务器行为、玩家行为、客户端行为等）
 * 
 * 那么按照这个思路，我们在服务端其实也是这么两种驱动：
 *  1、服务器自驱动：Scene 定时或者不定时的 Update（循环就是一种典型的自驱动）
 *  2、服务器外驱动：外部的协议导致服务器内部 Scene 发生变化（GM平台、玩家行为、开发操作、系统信号等）
 * 
 * 于是基于前面的结论，我想设计一个支持这两种驱动方式的 "游戏主场景"，本质上就是ECS架构
 */

namespace server::scene
{

const int GameSceneFrame = 20;  // 服务端游戏场景每秒20帧

/**
 * @brief 游戏服务器的根场景
 * 
 */
class GameServerScene:
    public engine::scene::Scene
{
    typedef engine::ecs::GameObjectSPtr    GameObjectSPtr;
public:
    GameServerScene();
    ~GameServerScene();

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
    engine::ecs::GameObjectSPtr DBServiceInit();
    /* 阻塞的等待IO线程退出 */
    void IOThreadExit();

    /* 场景中实例全部卸载后调用 */
    // void OnSceneEmpty();
    void OnStopScene();
private:
    std::shared_ptr<bbt::pollevent::EventLoop>              m_loop{nullptr};
    // std::shared_ptr<bbt::network::libevent::EventLoop>      m_loop{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_update_event{nullptr};
    std::shared_ptr<bbt::pollevent::Event>                  m_signal_sigint_handle{nullptr};
    volatile bool                                           m_is_stop{false};          // 是否停止            

    engine::ecs::GameObjectId                               m_network_id;
};

} // namespace end