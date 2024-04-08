#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>

namespace share::ecs::network
{

class Connection
{
public:
    Connection();
    ~Connection();
private:
    void OnRecv(const char* data, size_t len) {}
    void OnClose(void* udata, const bbt::net::IPAddress& addr) {}
    void OnSend(const bbt::network::Errcode& err, size_t send_len) {}
    void OnTimeout() {}
private:
    bbt::network::libevent::ConnCallbacks m_conn_callbacks;
    bbt::network::ConnId                  m_conn_id{0};
};

}