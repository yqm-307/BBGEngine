#pragma once
#include <event2/event.h>
#include <string>
#include <bbt/thread/Lock.hpp>
#include <bbt/Attribute.hpp>
#include "util/network/libevent/evIOThread.hpp"
#include "share/ecs/network/entity/Acceptor.hpp"
#include "engine/ecs/gameobject/GameObject.hpp"


namespace share::ecs::entity::network
{

void OnFixUpdate(evutil_socket_t,short,void*);

enum emNetworkRunStatus{
    Default = 0,    // 未开始
    Starting,       // io线程启动中
    Active,         // io线程活跃中
    Stopping,       // io线程关闭中
    Inactive,       // io线程不活跃
};

/**
 * @brief 网络层（转接器）
 * 
 * ps: 推荐使用方式为组合使用，不使用继承，性能更佳
 * 
 * 提供用户层的IO回调的封装，自定义的提供下列回调事件：
 *  1、OnConnect，新链接到达时触发；
 *  2、OnClose，连接关闭时触发；
 *  3、OnTimeOut，连接超时时触发；
 * 
 * 提供的接口：
 *  1、SyncStart，同步启动所有io线程；
 *  2、SyncStop，同步的停止所有io线程；
 *  3、IsStoped，io线程是否全部停止；
 *
 */
class Network:
    public engine::ecs::GameObject
{
    friend void OnFixUpdate(evutil_socket_t,short,void*);
    GameObjectDeriveClassDef;
public:
    typedef util::network::ev::evIOThreadSPtr evIOThreadSPtr;
    typedef std::function<void(util::network::ConnectionSPtr)> OnConnectCallback;
    typedef std::function<void(util::network::ConnectionSPtr)> OnAcceptCallback;
    typedef std::function<void(util::network::ConnectionSPtr)> OnCloseCallback;
    typedef std::function<void(util::network::ConnectionSPtr, time_t)> OnTimeOutCallback;

    ~Network();
    /* 同步的启动所有线程，等所有IO线程全部启动返回 */
    bool SyncStart();
    // /* 同步的停止所有IO线程，等所有IO线程全部停止后返回 */
    void SyncStop();
    /* 是否还有io线程在运行中 */
    bool IsStoped();
    BBTATTR_FUNC_Deprecated void SetOnConnect(const OnConnectCallback& cb);
    void SetOnAccept(const OnAcceptCallback& cb);
    
    BBTATTR_FUNC_Deprecated void SetOnClose(const OnCloseCallback& cb);
    BBTATTR_FUNC_Deprecated void SetOnTimeOut(const OnTimeOutCallback& cb);
private:
    Network(const std::string& ip, short port);
    /* 对象内部数据申请 */
    void Init();
    /* 对象内部数据释放 */
    void Destory();
    void TryStopAllThread();
    /* 等待所有IO线程，启动完成 */
    void WaitForOtherIOThreadStart();
private:
    virtual void OnUpdate();
    virtual void OnFatherDead() {}
    /* io 工作线程 */
    void IOWork(int index);
    /* accept 线程 */
    void AcceptWork(int index, evIOThreadSPtr this_thread);


    void RegistFixUpdate(int index);
    void OnFixUpdate(int index);
private:
    /**
     * utils
     */
    /* libevent event_base 构造 */
    event_base* OnCreateEventBase() BBTATTR_FUNC_RetVal;
    /* libevent event_base 释放 */
    void OnDestoryEventBase(event_base* base);
    evIOThreadSPtr NewConnLoadBlance() BBTATTR_FUNC_RetVal;
private:
    __detail::Acceptor m_acceptor;
    size_t          m_io_thread_num;
    std::vector<evIOThreadSPtr> m_io_threads;    // 下标0是acceptor线程
    std::vector<event_base*> m_ev_bases;    // 每个线程一个 event_base
    bbt::thread::lock::CountDownLatch* m_thread_latch;
    bool                m_is_in_loop{false};    // 运行状态
    bool                m_is_need_stop{false};  // 停止flag
    emNetworkRunStatus  m_status{emNetworkRunStatus::Default};   // 运行时状态

    /* io 回调 */
    OnConnectCallback   m_onconnect_event{nullptr};
    OnAcceptCallback    m_onaccept_event{nullptr};
    OnCloseCallback     m_onclose_event{nullptr};
    OnTimeOutCallback   m_ontimeout_event{nullptr};
};

}