#pragma once
#include <iostream>
#include <event2/event.h>
#include <optional>
#include "util/typedef/NamespaceType.hpp"
#include "util/network/IPAddress.hpp"

namespace game::util::network
{

class Connection;
SmartPtrTypeDef(Connection);

enum ConnStatus
{
    Done        = 0,        // 初始状态
    Connecting  = 1,        // 连接建立中
    Connected   = 2,        // 连接中
    Disconnected = 3,       // 连接已断开
};

class Connection
{
public:
    Connection(){}
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
     */
    virtual void Close() = 0;


    /**
     * @brief 获取对端的ip地址
     * @return Address 
     */
    virtual const Address& GetPeerIPAddress() const = 0;

    /**
     * @brief 获取本地的ip地址
     * 
     * @return Address 
     */
    virtual const Address& GetLocalIPAddress() const = 0;

private:

};


}