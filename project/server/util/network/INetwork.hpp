#pragma once
#include "util/network/Define.hpp"
#include "util/network/Connection.hpp"
#include "util/network/IOThread.hpp"

namespace util::network
{

class INetwork;

class INetCallback
{
public:
    /**
     * @brief 事件：当连接对端完成时执行
     * 
     * @param fd            此连接的sockfd
     * @param local_addr    本地的地址
     * @param peer_addr     对端的地址
     */
    virtual void OnConnect(ThreadId tid, int fd, const Address& local_addr, const Address& peer_addr) = 0;

    /**
     * @brief 事件：当连接关闭时执行
     * 
     * @param fd            此连接的sockfd
     * @param peer_addr     对端的地址
     */
    virtual void OnDisconnect(ThreadId tid, int fd, const Address& peer_addr) = 0;

    /**
     * @brief 事件：当有新的连接到本地时
     * 
     * @param fd            此连接的sockfd
     * @param peer_addr     对端的地址
     */
    virtual void OnAccept(IOThreadWKPtr io_thread, int fd, const Address& peer_addr) = 0;

    /**
     * @brief 事件：当sockfd接收到数据时执行
     * 
     * @param data  数据
     * @param len   接收到的数据长度
     */
    virtual void OnRecv(ThreadId tid, const char* data, size_t len) = 0;

    /**
     * @brief 事件：当sockfd发送完成时
     * 
     * @param len   发送成功的长度
     */
    virtual void OnSend(ThreadId tid, size_t len) = 0;
};

class INetwork
{
public:
    virtual ~INetwork() = 0;

    /* 初始化 */
    virtual void Init() = 0;
    /* 关闭网络处理 */
    virtual void Stop() = 0;
    /* 开启网络处理 */
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual bool IsRunning() = 0;

    /* 监听本地地址 */
    virtual errcode::ErrOpt Listen(Address listen_addr) = 0;
    /* 连接目标地址 */
    virtual errcode::ErrOpt Connect(Address peer_addr) = 0;
    /* 异步连接目标地址 */
    virtual errcode::ErrOpt AsyncConnect(Address peer_addr) = 0;
    /* 断开网络连接 */
    virtual errcode::ErrOpt Disconnect(ConnectionSPtr conn) = 0;
private:
};

}