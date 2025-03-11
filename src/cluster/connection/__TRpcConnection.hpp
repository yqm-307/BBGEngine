#pragma once
#include <cluster/connection/RpcConnection.hpp>
#include <cluster/protocol/Protocol.hpp>

namespace cluster
{

template<class TListener>
RpcConnection<TListener>::RpcConnection(emRpcConnType type, std::weak_ptr<TListener> listener, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms),
    m_listener_wptr(listener),
    m_type(type)
{
}

template<class TListener>
RpcConnection<TListener>::~RpcConnection()
{
}

template<class TListener>
emRpcConnType RpcConnection<TListener>::GetType() const
{
    return m_type;
}

template<class TListener>
void RpcConnection<TListener>::ProcessRecvBuffer()
{
    protocol::ProtocolHead* head = nullptr;
    std::vector<bbt::core::Buffer> buffers;
    
    while (m_recv_buffer.Size() >= sizeof(protocol::ProtocolHead))
    {
        head = (protocol::ProtocolHead*)m_recv_buffer.Peek();
        Assert(head->protocol_length <= protocol::MAX_PROTOCOL_LENGTH);   // 大概是有bug了
        if (m_recv_buffer.Size() < head->protocol_length)
            break;

        bbt::core::Buffer buffer{(size_t)head->protocol_length};
        Assert(buffer.WriteNull(head->protocol_length) == head->protocol_length);
        m_recv_buffer.ReadString(buffer.Peek(), head->protocol_length);
        buffers.emplace_back(std::move(buffer));
    }

    if (!buffers.empty())
        _SubmitRequest2Registery(buffers);

}

template<class TListener>
void RpcConnection<TListener>::_SubmitRequest2Registery(std::vector<bbt::core::Buffer>& buffer)
{
    auto listener = m_listener_wptr.lock();
    if (listener == nullptr)
        return;
    
    for (auto&& buf : buffer)
        listener->SubmitReq2Listener(GetConnId(), GetType(), buf);
}

template<class TListener>
void RpcConnection<TListener>::OnRecv(const char* data, size_t len)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recv_buffer.WriteString(data, len);

    if (!m_listener_wptr.expired()) {
        ProcessRecvBuffer();
    }
}

template<class TListener>
void RpcConnection<TListener>::OnSend(util::errcode::ErrOpt err, size_t len)
{
    auto listener = m_listener_wptr.lock();
    if (listener != nullptr)
        listener->NotifySend2Listener(GetConnId(), GetType(), err, len);
}

template<class TListener>
void RpcConnection<TListener>::OnClose()
{
    auto listener = m_listener_wptr.lock();
    if (listener != nullptr)
        listener->NotityOnClose2Listener(GetConnId(), GetType());
}

template<class TListener>
void RpcConnection<TListener>::OnTimeout()
{
    auto listener = m_listener_wptr.lock();
    if (listener != nullptr)
        listener->NotityOnTimeout2Listener(GetConnId(), GetType());
}

template<class TListener>
void RpcConnection<TListener>::OnError(const util::errcode::Errcode& err)
{
    auto listener = m_listener_wptr.lock();
    if (listener != nullptr)
        listener->OnError(err);
}


} // namespace cluster