#pragma once
#include "util/network/Connection.hpp"
#include "util/typedef/NamespaceType.hpp"
#include "util/network/IOThread.hpp"
#include "bbt/buffer/Buffer.hpp"

/**
 * @brief libevent实现
 * 
 */
namespace game::util::network
{
#define NET_HANDLER_ENTRY(errcode_key, capture, lambda_body) {errcode_key, [capture](const bbt::buffer::Buffer& buffer){lambda_body(buffer);}}

static const int evConnection_Timeout_MS = 3000;

void OnRecvCallback(int sockfd, short events, void* args);
void OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);

namespace ev
{

class evIOThread;
class evConnection;
SmartPtrTypeDef(evConnection);

/**
 * @brief 可靠的双向连接，基于Tcp
 */
class evConnection:
    public Connection
{
    friend void game::util::network::OnRecvCallback(int sockfd, short events, void* args);
    friend void game::util::network::OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);
    friend class evConnMgr;

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

public:
    virtual ~evConnection();

    virtual bool IsClosed() override;
    virtual void Close() override;
    virtual size_t Send(const char* buffer, size_t len) override;
    virtual size_t Recv(const char* buffer, size_t len) override;

    virtual const Address& GetPeerIPAddress() const override;
    virtual const Address& GetLocalIPAddress() const override;
    /* 获取所在的 event_base */
    const event_base*                   GetEvBase() const;
    std::pair<char*,size_t>             GetRecvBuffer();
protected:

    /**
     * @brief 构造一个新连接
     * 
     * @param thread 运行在的io线程上
     * @param newfd 套接字
     * @param peer_ip 对端socket地址
     * @param local_ip 本地socket地址
     */
    evConnection(IOThread* thread, int newfd, Address peer_ip, Address local_ip);
    
    void Init();
    void Destroy();
private:
    /* IO事件初始化 */
    void InitEvent();
    /* read事件派发函数，read事件有很多可能eof、refused等，所以需要通过此函数派发到对应的事件处理函数 */
    void OnRecvEventDispatch(const bbt::buffer::Buffer& buffer, const util::errcode::ErrCode& err);
    /* 获取当前连接所在的IO线程 */
    evIOThread* GetIOThread();

private:
    //----------- NetWork Handler -------------//
    void NetHandler_RecvData(const bbt::buffer::Buffer& buffer);
    void NetHandler_ConnClosed(const bbt::buffer::Buffer& buffer);
    void NetHandler_TryAgain(const bbt::buffer::Buffer& buffer);
    void NetHandler_OtherErr(const bbt::buffer::Buffer& buffer);

    //----------- timeout Handler -------------//
    void TimeOutHandler(const util::errcode::ErrCode& err);
private:

    IOThread*   m_io_thread;
    event_base* m_ev_base;
    int         m_sockfd;
    ConnStatus  m_status;
    event*      m_recv_event;   // 接收事件
    event*      m_timeout_event;// 超时事件
    // event*      m_ev_send;
    util::network::Address  m_local_addr;
    util::network::Address  m_peer_addr;

    char        m_recv_buffer[4096];    // socket 接收缓存，后续可以接入配置中
};
}

}

#ifdef NET_HANDLER_ENTRY
#undef NET_HANDLER_ENTRY
#endif