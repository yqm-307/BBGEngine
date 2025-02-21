#include <util/log/Log.hpp>
#include <util/network/Connection.hpp>

namespace util::network
{

uint64_t Connection::m_id_template = 1;

Connection::Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    m_raw_conn_ptr(raw_conn),
    m_conn_id(GenerateId())
{
    m_conn_callbacks.on_close_callback = [this](void* udata, const bbt::net::IPAddress& addr){ _OnClose(); };
    m_conn_callbacks.on_err_callback = [this](void* udata, const bbt::errcode::Errcode& err){ OnError(err); };
    m_conn_callbacks.on_recv_callback = [this](bbt::network::libevent::ConnectionSPtr conn, const char* data, size_t len){ OnRecv(data, len); };
    m_conn_callbacks.on_send_callback = [this](bbt::network::libevent::ConnectionSPtr conn, const bbt::errcode::Errcode& err, size_t len){ OnSend(err, len); };
    m_conn_callbacks.on_timeout_callback = [this](bbt::network::libevent::ConnectionSPtr conn){ OnTimeout();};

    Assert(timeout_ms > 0);
    raw_conn->SetOpt_CloseTimeoutMS(timeout_ms);
    raw_conn->SetOpt_Callbacks(m_conn_callbacks);
}

Connection::~Connection()
{
}

void Connection::_OnClose()
{
    if (m_onclose_cb != nullptr)
        m_onclose_cb(m_conn_id);

    OnClose();
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

void Connection::SetOnClose(std::function<void(bbt::network::ConnId)> onclose)
{
    m_onclose_cb = onclose;
}


}