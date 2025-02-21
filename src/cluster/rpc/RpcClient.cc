#include <cluster/rpc/RpcClient.hpp>

namespace cluster
{

RpcClient::~RpcClient()
{
}

RpcClient::RpcClient()
{
}

void RpcClient::OnReply(const char* data, size_t size)
{
    bbt::core::Buffer buffer(data, size);
    FieldHeader header;
    if (buffer.ReadableBytes() < sizeof(header))
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

    if (buffer.ReadableBytes() < header.field_len)
    {
        GAME_EXT1_LOG_ERROR("a bad protocol! buffer not enough %d byte!", header.field_len);
        return;
    }
    int seq = buffer.ReadInt64();

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

} // namespace plugin::ecs::rpc