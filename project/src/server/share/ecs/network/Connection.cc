#include "util/log/Log.hpp"
#include "share/ecs/network/Connection.hpp"
#include "share/ecs/network/ConnMgr.hpp"

namespace share::ecs::network
{

uint64_t Connection::m_id_template = 1;

Connection::Connection(ConnMgr* mgr, bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    m_raw_conn_ptr(raw_conn),
    m_conn_id(GenerateId()),
    m_conn_mgr(mgr)
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
    m_conn_mgr->DelConnect(m_conn_id);
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
    GAME_EXT1_LOG_WARN("[Connection::OnSend] no onsend %ld", succ_len);
}

void Connection::OnTimeout()
{
    GAME_EXT1_LOG_WARN("[Connection::OnTimeout] ontimeout!");
}

bbt::network::ConnId Connection::GetConnId()
{
    return m_conn_id;
}

void Connection::Send(const char* data, size_t len)
{
    m_raw_conn_ptr->AsyncSend(data, len);
}

bool Connection::IsClosed()
{
    return (m_raw_conn_ptr == nullptr) ? false : m_raw_conn_ptr->IsClosed();
}

void Connection::Close()
{
    if (m_raw_conn_ptr == nullptr)
        return;
    
    m_raw_conn_ptr->Close();
}


}