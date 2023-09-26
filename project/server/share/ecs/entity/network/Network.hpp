#pragma once
#include <event2/event.h>
#include <string>
#include <bbt/thread/Lock.hpp>
#include "util/network/libevent/evIOThread.hpp"
#include "share/ecs/entity/network/Acceptor.hpp"
#include "engine/ecs/gameobject/GameObject.hpp"


namespace share::ecs::entity::network
{

// FIXME 修改为gameobject实现，此模块目前实现错误，修复后删除
class Network:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    typedef util::network::ev::evIOThread IOThread;

    ~Network();
    /* 同步的启动所有线程，等所有IO线程全部启动返回 */
    void SyncStart();
    /* 异步的停止所有IO线程，立即返回 */
    void AsyncStop();
    /* 同步的停止所有IO线程，等所有IO线程全部停止后返回 */
    void SyncStop();
private:
    Network(const std::string& ip, short port);
    /* 对象内部数据申请 */
    void Init();
    /* 对象内部数据释放 */
    void Destory();
    /* io 工作线程 */
    void IOWork(int index);
    /* accept 线程 */
    void AcceptWork(int index);
    /* 等待所有IO线程，启动完成 */
    void WaitForOtherIOThreadStart();
private: /* 继承接口 */
    virtual void OnUpdate();
    virtual void OnFatherDead() {}
private:
    /**
     * utils
     */
    /* libevent event_base 构造 */
    event_base* OnCreateEventBase();
    /* libevent event_base 释放 */
    void OnDestoryEventBase(event_base* base);
    util::network::IOThread* NewConnLoadBlance();
private:
    __detail::Acceptor m_acceptor;
    size_t          m_io_thread_num;
    std::vector<IOThread*> m_io_threads;    // 下标0是acceptor线程
    std::vector<event_base*> m_ev_bases;    // 每个线程一个 event_base
    bbt::thread::lock::CountDownLatch* m_thread_latch;
    bool            m_is_in_loop{false};
    bool            m_is_need_stop{false};
};

}