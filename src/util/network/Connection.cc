#include <util/log/Log.hpp>
#include <util/network/Connection.hpp>

namespace util::network
{

Connection::Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    m_raw_conn_ptr(raw_conn)
{
    Assert(timeout_ms > 0);
    raw_conn->SetOpt_CloseTimeoutMS(timeout_ms);
}

Connection::~Connection()
{
}

void Connection::Init()
{
    if (m_raw_conn_ptr == nullptr)
        return;
    
    m_raw_conn_ptr->SetOpt_Callbacks({
        .on_recv_callback = [weak_this{weak_from_this()}](auto, const char* data, size_t len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->OnRecv(data, len);
        },
        .on_send_callback = [weak_this{weak_from_this()}](auto, auto err, size_t succ_len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->OnSend(err, succ_len);
        },
        .on_close_callback = [weak_this{weak_from_this()}](auto, auto)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->_OnClose();
        },
        .on_timeout_callback = [weak_this{weak_from_this()}](auto)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->OnTimeout();
        },
        .on_err_callback = [weak_this{weak_from_this()}](void*, const util::errcode::Errcode& err)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr)
                shared_this->OnError(err);
        },
    });
}


bbt::network::ConnId Connection::GetConnId()
{
    return m_raw_conn_ptr->GetConnId();
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

const util::network::IPAddress& Connection::GetPeerAddr() const
{
    return m_raw_conn_ptr->GetPeerAddress();
}

void Connection::_OnClose()
{
    if (m_notify_close_to_tcpserver_cb != nullptr)
        m_notify_close_to_tcpserver_cb(GetConnId());

    OnClose();
}

void Connection::SetOnCloseNotifyToTcpServer(std::function<void(bbt::network::ConnId)> onclose)
{
    m_notify_close_to_tcpserver_cb = onclose;
}
}