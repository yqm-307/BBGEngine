#pragma once
#include <iostream>
#include <event2/event.h>
#include <optional>
#include "util/typedef/NamespaceType.hpp"
#include "util/network/IPAddress.hpp"
#include "./Define.hpp"

namespace util::network
{

class Connection;
SharedWithUniqueDef(Connection);


enum ConnStatus
{
    Done        = 0,        // 初始状态
    Connecting  = 1,        // 连接建立中
    Connected   = 2,        // 连接中
    Disconnected = 3,       // 连接已断开
};

class Connection:
    public util::managerbase::MemberBase<ConnectId, Connection>,
    public std::enable_shared_from_this<Connection>
{
public:
    Connection(int sockfd, const Address& peer, const Address& local);
    virtual ~Connection() = 0;

    /**
     * @brief 连接是否关闭了
     * 
     * @return true 
     * @return false 
     */
    virtual bool IsClosed() = 0;

    /**
     * @brief 向连接对端发送len个字节的buffer（异步）
     * 
     * @param buffer 待发送数据
     * @param len 长度
     * @return size_t 发送成功的数据长度
     */
    virtual size_t Send(const char* buffer, size_t len) = 0;

    /**
     * @brief 接收数据到buffer中，最多len字节
     * 
     * @param buffer 缓冲区地址
     * @param len 缓冲区长度
     * @return size_t 成功接收长度
     */
    virtual size_t Recv(const char* buffer, size_t len) = 0;

    /**
     * @brief 关闭tcp连接
     * 对于关闭连接的要求：
     * 1、释放掉系统资源（套接字、内存等）
     * 2、标记连接的其他接口（IO接口）不可被使用
     * 3、避免在类被析构时重复释放
     */
    virtual void Close() = 0;


    /**
     * @brief 获取对端的ip地址
     * @return Address 
     */
    virtual const Address& GetPeerIPAddress() const;

    /**
     * @brief 获取本地的ip地址
     * 
     * @return Address 
     */
    virtual const Address& GetLocalIPAddress() const;

    /**
     * @brief 获取原始套接字
     * 
     * @return int 
     */
    int GetSocket() const;

protected:

    int                     m_sockfd{-1};
    util::network::Address  m_local_addr;
    util::network::Address  m_peer_addr;

    /* 保证可见性 */
    volatile ConnStatus     m_status{ConnStatus::Done};
    /* 去除对连接状态保护的锁，因为不太可能多线程同时析构，那样更严重 */
    // std::mutex              m_mutex; 
};


}