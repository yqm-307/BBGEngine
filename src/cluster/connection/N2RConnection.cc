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

void N2RConnection::OnSend(util::errcode::ErrOpt err, size_t len)
{
    auto node = m_node.lock();
    if (node)
        node->OnSendToRegistery(err, len);
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

void N2RConnection::OnError(const util::errcode::Errcode& err)
{
    auto node = m_node.lock();
    if (node)
        node->OnError(err);
}

} // namespace cluster