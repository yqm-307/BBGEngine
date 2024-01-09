#pragma once
#include <thread>
#include <functional>
#include "util/network/Connection.hpp"
#include "util/errcode/ErrCode.hpp"
#include "util/network/libevent/evConnection.hpp"


namespace util::network
{

/* 因为本身 OnRecv 函数带状态，所以只能用全局的void(void*)来包裹 */

void OnConnectCallback(evutil_socket_t sockfd, short events, void* args);

void OnAcceptCallback(evutil_socket_t sockfd, short events, void* args);


/**
 * @brief 创建一个监听套接字。
 * 
 * @param ip 监听地址
 * @param port 监听端口
 * @param noblock 是否设置非阻塞
 * @return evutil_socket_t 成功返回不小于0的fd，失败返回-1
 */
evutil_socket_t CreateListen(std::string ip, short port, bool noblock);

int SetFdNoBlock(int fd) BBTATTR_FUNC_RetVal;

int SetFdReUseAddr(int fd) BBTATTR_FUNC_RetVal;

void OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);

}