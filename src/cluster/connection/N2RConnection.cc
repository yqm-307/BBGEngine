#include <cluster/node/ClusterNode.hpp>
#include <cluster/connection/N2RConnection.hpp>

namespace cluster
{

N2RConnection::N2RConnection(std::weak_ptr<ClusterNode> node, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms),
    m_node(node)
{
}

N2RConnection::~N2RConnection()
{
}

void N2RConnection::ProcessRecvBuffer()
{
    R2NProtocolHead* head = nullptr;
    std::vector<bbt::core::Buffer> buffers;
    
    while (m_recv_buffer.DataSize() > sizeof(R2NProtocolHead))
    {
        head = (R2NProtocolHead*)m_recv_buffer.Peek();
        if (m_recv_buffer.DataSize() < head->protocol_length)
            return;
        
        if (head->protocol_type > R2N_PROTOCOL_NONE && head->protocol_type < R2N_PROTOCOL_SIZE)
            return;
            
        bbt::core::Buffer buffer{(size_t)head->protocol_length};
        Assert(buffer.WriteNull(head->protocol_length) == head->protocol_length);
        m_recv_buffer.ReadString(buffer.Peek(), head->protocol_length);
        buffers.emplace_back(std::move(buffer));
    }

    _SubmitRequest2Node(buffers);
}

void N2RConnection::_SubmitRequest2Node(std::vector<bbt::core::Buffer>& buffer)
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    for (auto&& buf : buffer)
        node->RequestFromRegistery(buf);
}

void N2RConnection::OnRecv(const char* data, size_t len)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recv_buffer.WriteString(data, len);

    if (!m_node.expired()) {
        ProcessRecvBuffer();
    }
}

void N2RConnection::OnSend(bbt::errcode::ErrOpt err, size_t len)
{
}

void N2RConnection::OnClose()
{
    auto node = m_node.lock();
    if (node)
        node->OnCloseFromRegistery(GetConnId(), GetPeerAddr());
}

void N2RConnection::OnTimeout()
{
    auto node = m_node.lock();
    if (node)
        node->OnTimeoutFromRegistey(GetConnId());
}

void N2RConnection::OnError(const bbt::errcode::Errcode& err)
{
    auto node = m_node.lock();
    if (node)
        node->OnError(err);
}

} // namespace cluster