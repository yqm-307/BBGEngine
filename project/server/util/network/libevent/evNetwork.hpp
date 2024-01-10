#pragma once
#include "util/network/INetwork.hpp"
#include "util/network/libevent/Acceptor.hpp"
#include "evIOCallbacks.hpp"

namespace util::network::ev
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
class evNetwork:
    public INetwork
{
    friend void OnUpdate(evutil_socket_t, short, void*);
public:
    typedef util::network::ev::evIOThreadSPtr                           evIOThreadSPtr;
    typedef std::function<void(util::network::ConnectionSPtr)>          OnConnectCallback;
    typedef std::function<void(util::network::ConnectionSPtr)>          OnAcceptCallback;
    typedef std::function<void(util::network::ConnectionSPtr)>          OnCloseCallback;
    typedef std::function<void(util::network::ConnectionSPtr, time_t)>  OnTimeOutCallback;

    evNetwork(const std::string& ip, short port);
    virtual ~evNetwork();
    void SetCallback(evNetCallback* callback);
    /* 同步的启动所有线程，等所有IO线程全部启动返回 */
    virtual void Start() override;
    /* 同步的停止所有IO线程，等所有IO线程全部停止后返回 */
    virtual void Stop() override;
    /* 是否还有io线程在运行中 */
    virtual bool IsRunning() override;
    virtual void Update() override;
    virtual errcode::ErrOpt Listen(Address listen_addr) override;
    virtual errcode::ErrOpt Connect(Address perr_addr) override;
    virtual errcode::ErrOpt AsyncConnect(Address peer_addr) override;
    virtual errcode::ErrOpt Disconnect(ConnectionSPtr conn) override;
private:
    detail::Acceptor    m_acceptor;
    INetCallback*       m_callback;
};


}