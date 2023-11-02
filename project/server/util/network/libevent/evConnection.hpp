#pragma once
#include <functional>
#include "util/network/Connection.hpp"
#include "util/typedef/NamespaceType.hpp"
#include "util/network/IOThread.hpp"
#include "util/network/libevent/evEvent.hpp"
#include "util/network/libevent/evConnMgr.hpp"
#include <bbt/buffer/Buffer.hpp>
#include <bbt/timer/Clock.hpp>

/**
 * @brief libevent实现
 * 
 */
namespace util::network
{
#define NET_HANDLER_ENTRY(errcode_key, capture, lambda_body) {errcode_key, [capture](const bbt::buffer::Buffer& buffer){lambda_body(buffer);}}

static const int evConnection_Timeout_MS = 3000;

void OnRecvCallback(int sockfd, short events, void* args);
void OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);

namespace ev
{

/* libevent 到 C++闭包对象中间层，希望使用函数对象统一io操作 */
typedef std::function<void(evutil_socket_t, short, void*)> IOCommCallback;

struct evArgs
{
    IOCommCallback  do_io_callback{nullptr};
};



class evIOThread;
class evConnection;
class evConnMgr;
OnlySharedDef(evConnection);

/**
 * @brief 可靠的双向连接，基于Tcp
 * 
 * 注意：evConnection的对外接口大多都是来自多线程调用的
 */
class evConnection:
    public Connection
{
    evConnectionDeriveClassDef;
    friend void util::network::OnRecvCallback(int sockfd, short events, void* args);
    friend class evConnMgr;

    typedef std::function<void(evConnectionSPtr)>   OnDestoryCallback;
    typedef std::function<bool(evConnectionSPtr)>   OnTimeOutCallback;
    typedef std::function<void(const util::errcode::ErrCode&, size_t)>  OnRecvCallback;
    typedef std::function<void(const util::errcode::ErrCode&, size_t)>  OnSendCallback;
public:

    virtual ~evConnection();

    virtual bool IsClosed() override;
    virtual void Close() override;
    /**
     * @brief 
     * 
     * @param buffer 
     * @param len 
     * @return size_t 返回写入成功的字节数，如果为0说明连接已经断开
     */
    virtual size_t Send(const char* buffer, size_t len) override;
    /**
     * @brief 从本连接的接收缓存里获取len字节写入到buffer中
     * 
     * @param buffer 保存的值
     * @param len   获取的字节数
     * @return size_t 写入读取成功的字节数，如果为0说明连接已经断开
     */
    virtual size_t Recv(const char* buffer, size_t len) override;

    /* 设置一个回调，回调在从socket保存数据到当前连接的缓存后调用 */
    void SetOnRecvCallback(const OnRecvCallback& onrecv_handler);
    /* 设置一个回调，回调在通过socket发送到网络上后调用 */
    void SetOnSendCallback(const OnSendCallback& onsend_handler);

    virtual const Address& GetPeerIPAddress() const override;
    virtual const Address& GetLocalIPAddress() const override;
    std::pair<char*,size_t> GetRecvBuffer();

    void OnHeartBeat();
private:
    /**
     * @deprecated 不允许被随意使用
     * @brief 构造一个新连接
     * 
     * @param thread 运行在的io线程上
     * @param newfd 套接字
     * @param peer_ip 对端socket地址
     * @param local_ip 本地socket地址
     */
    evConnection(evIOThreadSPtr thread, int newfd, Address peer_ip, Address local_ip);
    void OnDestroy();

    /* IO事件初始化 */
    void InitEvent();
    void InitEventArgs();
    evConnectionSPtr SPtr();
private:
    //----------------- Read Only -------------------//
    /* 获取当前连接所在的IO线程 */
    evIOThreadSPtr GetIOThread();
    /* 获取当前连接的套接字 */
    evutil_socket_t GetSocket();
    /* 获取当前连接上次心跳包的时间 */
    bbt::timer::clock::Timestamp<bbt::timer::clock::ms>
        GetPrevHeartBeatTimestamp();
private:
    //----------- OnEvent Dispatcher：监听事件分发  -------------//
    /* 连接的总监听事件 */
    void OnEvent(evutil_socket_t fd, short events, void* args);

    void EventHandler_OnRecv(evutil_socket_t fd, short events, void* args);
    void EventHandler_OnSocketTimeOut(evutil_socket_t fd, short events, void* args);
    void EventHandler_OnClose(evutil_socket_t fd, short events, void* args);

    //----------- IO Dispatcher：IO事件分发  -------------//
    /* IO事件总监听事件 */
    void OnRecvEventDispatch(const bbt::buffer::Buffer& buffer, const util::errcode::ErrCode& err);

    //----------- IO Dispatcher  -------------//
    void OnRecv(evutil_socket_t fd, short events, void* args);
    // void OnSend(evutil_socket_t fd, short evebts, void* args);


    void NetHandler_RecvData(const bbt::buffer::Buffer& buffer);
    void NetHandler_ConnClosed(const bbt::buffer::Buffer& buffer);
    void NetHandler_TryAgain(const bbt::buffer::Buffer& buffer);
    void NetHandler_OtherErr(const bbt::buffer::Buffer& buffer);

    //----------- timeout Handler -------------//
    // 心跳的实现修改，由上层解析完心跳协议后，调用到Conenction更新心跳时间
    const std::map<int, 
        std::function<void(const bbt::buffer::Buffer&)>> m_errcode_handler
    {
        // 使用宏减少一些代码量，而且显而易见的地方使用宏感觉没啥问题
        // {util::errcode::network::err::Recv_Success, 
        // [this](const bbt::buffer::Buffer& buffer, const util::errcode::ErrCode& err){ NetHandler_RecvData(buffer, err); }},
        NET_HANDLER_ENTRY(util::errcode::network::err::Recv_Success,    this, NetHandler_RecvData),
        NET_HANDLER_ENTRY(util::errcode::network::err::Recv_Eof,        this, NetHandler_ConnClosed),
        NET_HANDLER_ENTRY(util::errcode::network::err::Recv_TryAgain,   this, NetHandler_TryAgain),
        NET_HANDLER_ENTRY(util::errcode::network::err::Recv_Other_Err,  this, NetHandler_OtherErr),
    };
private:

    evIOThreadWKPtr   m_io_thread;
    std::shared_ptr<evEvent>    m_recv_event{nullptr};      // 接收事件    
    std::shared_ptr<evEvent>    m_socket_timeout{nullptr};      // 接收事件    

    // char        m_recv_buffer[4096];    
    bbt::buffer::Buffer     m_recv_buffer;  // socket 接收缓存

    evArgs m_onrecv_args;

    /** 
     * 上次接收到心跳包的时间戳，默认为连接建立的时间。 
     * 
     * @yqm-307 关于线程安全，这个值在创建时是被认为只在多线程读取
     * ，不会多线程进行写操作。并且心跳对于线程之间的低概率
     * 延迟是不关心的，误差及其小。
     */
    bbt::timer::clock::Timestamp<bbt::timer::clock::ms>    
                m_prev_heart_beat_time;

    OnRecvCallback      m_onrecv{nullptr};
    OnSendCallback      m_onsend{nullptr};
    OnTimeOutCallback   m_timeout_cb{nullptr};
};
}

}

#ifdef NET_HANDLER_ENTRY
#undef NET_HANDLER_ENTRY
#endif