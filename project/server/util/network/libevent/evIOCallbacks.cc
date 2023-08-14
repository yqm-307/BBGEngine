#include "util/network/libevent/evIOCallbacks.hpp"

namespace game::util::network
{

[[maybe_unused]] void OnConnectCallback(int sockfd, short events, void* args)
{
    evIOCallbacks* ev_cb = static_cast<evIOCallbacks*>(args);
    OnConnect<ev::evConnection>(ev_cb->m_conn_ptr, ev_cb->err);
}

[[maybe_unused]] void OnAcceptCallback(int sockfd, short events, void* args)
{
    evIOCallbacks* ev_cb = static_cast<evIOCallbacks*>(args);
    OnAccept<ev::evConnection>(ev_cb->m_conn_ptr, ev_cb->err);
}

[[maybe_unused]] void OnRecvCallback(int sockfd, short events, void* args)
{
    evIOCallbacks* ev_cb = static_cast<evIOCallbacks*>(args);
    ev_cb->m_conn_ptr->OnRecv(sockfd);
}

}// namespace end