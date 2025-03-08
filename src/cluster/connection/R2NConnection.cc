#include <cluster/connection/R2NConnection.hpp>
#include <cluster/registery/Registery.hpp>


namespace cluster
{

R2NConnection::R2NConnection(std::weak_ptr<Registery> registery, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms),
    m_registery(registery)
{
}

R2NConnection::~R2NConnection()
{
}

void R2NConnection::ProcessRecvBuffer()
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

void R2NConnection::_SubmitRequest2Registery(std::vector<bbt::core::Buffer>& buffer)
{
    auto registery = m_registery.lock();
    if (m_registery.expired())
        return;
    
    for (auto&& buf : buffer)
        registery->OnRequest(GetConnId(), buf);
}

void R2NConnection::OnRecv(const char* data, size_t len)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recv_buffer.WriteString(data, len);

    if (!m_registery.expired()) {
        ProcessRecvBuffer();
    }
}

void R2NConnection::OnSend(util::errcode::ErrOpt err, size_t len)
{
    auto registery = m_registery.lock();
    if (registery != nullptr)
        registery->OnSendToNode(err, len);
}

void R2NConnection::OnClose()
{
    auto registery = m_registery.lock();
    if (registery != nullptr)
        registery->OnClose(GetConnId());
}

void R2NConnection::OnTimeout()
{
    auto registery = m_registery.lock();
    if (registery != nullptr)
        registery->OnTimeout(GetConnId());
}

void R2NConnection::OnError(const util::errcode::Errcode& err)
{
    auto registery = m_registery.lock();
    if (registery != nullptr)
        registery->OnError(err);
}


} // namespace cluster