#include "util/network/libevent/evIOCallbacks.hpp"

namespace game::util::network
{

[[maybe_unused]] void OnConnectCallback(evutil_socket_t sockfd, short events, void* args)
{
    evArgs* ev_cb = static_cast<evArgs*>(args);
    OnConnect<ev::evConnection>(ev_cb->m_conn_ptr, ev_cb->err);
}

[[maybe_unused]] void OnAcceptCallback(evutil_socket_t sockfd, short events, void* args)
{
    evArgs* ev_cb = static_cast<evArgs*>(args);
    OnAccept<ev::evConnection>(ev_cb->m_conn_ptr, ev_cb->err);
}

[[maybe_unused]] void OnRecvCallback(evutil_socket_t sockfd, short events, void* args)
{
    evArgs* ev_cb = static_cast<evArgs*>(args);
    ev_cb->m_conn_ptr->OnRecv(sockfd);
}

}// namespace end