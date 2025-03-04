#include <cluster/rpc/RpcClient.hpp>

namespace cluster
{

RpcClient::~RpcClient()
{
}

RpcClient::RpcClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const char* ip, short port, int timeout):
    util::network::TcpClient(network, ip, port, timeout)
{
}

void RpcClient::OnReply(const char* data, size_t size)
{
    bbt::core::Buffer buffer(data, size);
    FieldHeader header;
    if (buffer.Size() < sizeof(header))
    {
        GAME_EXT1_LOG_ERROR("a bad protocol! too short");
        return;
    }

    buffer.ReadString((char*)&header, sizeof(header));
    if (header.field_type != INT64)
    {
        GAME_EXT1_LOG_ERROR("a bad protocol! seq must be int64");
        return;
    }

    if (buffer.Size() < header.field_len)
    {
        GAME_EXT1_LOG_ERROR("a bad protocol! buffer not enough %d byte!", header.field_len);
        return;
    }

    int64_t seq;
    buffer.Read(seq);

    auto iter = m_callbacks.find(seq);
    if (iter == m_callbacks.end())
    {
        return;
    }

    auto err = iter->second(buffer);
    if (err)
    {
        GAME_EXT1_LOG_ERROR("rpc call error! %s", err->CWhat());
    }
}

int RpcClient::Send(const bbt::core::Buffer& buffer)
{
    auto conn = GetConn();
    if (conn->IsClosed()) {
        conn = nullptr;
        return -1;
    }

    if (conn != nullptr && !conn->IsClosed()) {
        conn->Send(buffer.Peek(), buffer.Size());
    }

    return -1;
}


} // namespace plugin::ecs::rpc