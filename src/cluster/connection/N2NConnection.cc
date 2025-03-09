#include <cluster/node/ClusterNode.hpp>
#include <cluster/connection/N2NConnection.hpp>

namespace cluster
{

N2NConnection::N2NConnection(std::weak_ptr<ClusterNode> node, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms),
    m_node(node)
{
}

N2NConnection::~N2NConnection()
{
}

void N2NConnection::ProcessRecvBuffer()
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

void N2NConnection::_SubmitRequest2Node(std::vector<bbt::core::Buffer>& buffer)
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    for (auto&& buf : buffer)
        node->RequestFromNode(buf);
}

void N2NConnection::OnRecv(const char* data, size_t len)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_recv_buffer.WriteString(data, len);
    if (!m_node.expired()) {
        ProcessRecvBuffer();
    }
}

void N2NConnection::OnSend(util::errcode ::ErrOpt err, size_t len)
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    if (err.has_value()) {
        node->OnInfo("N2NConnection::OnSend err: " + err.value().What());
    }
}

void N2NConnection::OnClose()
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    node->OnCloseFromNode(GetConnId());
}

void N2NConnection::OnTimeout()
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    node->OnTimeoutFromNode(GetConnId());
}

void N2NConnection::OnError(const util::errcode::Errcode& err)
{
    auto node = m_node.lock();
    if (m_node.expired())
        return;
    
    node->OnError(err);
}



} // namespace cluster