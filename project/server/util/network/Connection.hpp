#pragma once
#include <iostream>
#include <event2/event.h>
#include <optional>
#include "util/typedef/NamespaceType.hpp"

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
     * @brief 关闭tcp连接
     */
    virtual void Close() = 0;

    /**
     * @brief 获取对端ip地址
     * 
     * @return std::string 对端地址
     */
    virtual std::optional<std::string> GetPeerIP() = 0;

    /**
     * @brief 获取对端端口
     * 
     * @return int 对端端口
     */
    virtual std::optional<int> GetPeerPort() = 0;

    /**
     * @brief 获取本地监听ip
     * 
     * @return std::string 本地监听ip
     */
    virtual std::optional<std::string> GetLocalIP() = 0;

    /**
     * @brief 获取本地监听端口
     * 
     * @return int 本地监听端口
     */
    virtual std::optional<int> GetLocalPort() = 0;

private:

};


}