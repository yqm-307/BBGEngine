#include "share/ecs/network/Connection.hpp"
#include "util/log/Log.hpp"

namespace share::ecs::network
{

Connection::Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    m_raw_conn_ptr(raw_conn)
{
    m_conn_callbacks.on_close_callback = [this](void* udata, const bbt::net::IPAddress& addr){ OnClose(); };
    m_conn_callbacks.on_err_callback = [this](void* udata, const bbt::network::Errcode& err){ OnError(err); };
    m_conn_callbacks.on_recv_callback = [this](bbt::network::libevent::ConnectionSPtr conn, const char* data, size_t len){ OnRecv(data, len); };
    m_conn_callbacks.on_send_callback = [this](bbt::network::libevent::ConnectionSPtr conn, const bbt::network::Errcode& err, size_t len){ OnSend(err, len); };
    m_conn_callbacks.on_timeout_callback = [this](bbt::network::libevent::ConnectionSPtr conn){ OnTimeout();};

    Assert(timeout_ms > 0);
    raw_conn->SetOpt_CloseTimeoutMS(timeout_ms);
    raw_conn->SetOpt_Callbacks(m_conn_callbacks);
}

Connection::~Connection()
{
}

void Connection::OnClose()
{
    GAME_EXT1_LOG_WARN("no onclose!");
}

void Connection::OnError(const bbt::network::Errcode& err)
{
    GAME_EXT1_LOG_ERROR(err.CWhat());
}

void Connection::OnRecv(const char* data, size_t len)
{
    GAME_EXT1_LOG_WARN("[Connection::OnRecv] no onrecv %s", data);
}

void Connection::OnSend(const bbt::network::Errcode& err, size_t succ_len)
{
    GAME_EXT1_LOG_WARN("[Connection::OnSend] no onrecv %ld", succ_len);
}

void Connection::OnTimeout()
{
    GAME_EXT1_LOG_WARN("[Connection::OnTimeout] ontimeout!");
}



}