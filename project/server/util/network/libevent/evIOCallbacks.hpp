#pragma once
#include <thread>
#include <functional>
#include "util/network/IOCallbacks.hpp"
#include "util/network/libevent/evConnection.hpp"


namespace util::network
{

template<>
inline void OnAccept<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    GAME_EXT1_LOG_ERROR("OnAccept 对于 evConnection 的专用化!");
    auto ev_conn = std::static_pointer_cast<ev::evConnection>(new_conn);
}

template<>
inline void OnConnect<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    GAME_EXT1_LOG_ERROR("OnConnect 对于 evConnection 的专用化!");
}

/* 因为本身 OnRecv 函数带状态，所以只能用全局的void(void*)来包裹 */

void OnConnectCallback(evutil_socket_t sockfd, short events, void* args);

void OnAcceptCallback(evutil_socket_t sockfd, short events, void* args);

void OnRecvCallback(evutil_socket_t sockfd, short events, void* args);

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