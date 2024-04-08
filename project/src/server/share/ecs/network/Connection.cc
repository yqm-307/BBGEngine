#include "share/ecs/network/Connection.hpp"

namespace share::ecs::network
{

Connection::Connection()
{
    m_conn_callbacks.on_close_callback = [](void* udata, const bbt::net::IPAddress& addr){

    };

    m_conn_callbacks.on_err_callback = [](void* udata, const bbt::network::Errcode& err){

    };

    m_conn_callbacks.on_recv_callback = [](bbt::network::libevent::ConnectionSPtr conn, const char* data, size_t len){

    };

    m_conn_callbacks.on_send_callback = [](bbt::network::libevent::ConnectionSPtr conn, const bbt::network::Errcode& err, size_t send_len){

    };

    m_conn_callbacks.on_timeout_callback = [](bbt::network::libevent::ConnectionSPtr conn){

    };   
}

Connection::~Connection()
{
}

}