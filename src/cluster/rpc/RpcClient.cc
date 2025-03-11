#include <cluster/rpc/RpcClient.hpp>
#include <cluster/protocol/Protocol.hpp>

namespace cluster
{

using namespace protocol;

RpcClient::~RpcClient()
{
}

RpcClient::RpcClient():
    m_network(std::make_shared<bbt::network::libevent::Network>())
{
}

util::errcode::ErrOpt RpcClient::Init(const util::network::IPAddress& registery_addr, int timeout_ms)
{
    
}

void RpcClient::OnReply(const char* data, size_t size)
{
    bbt::core::Buffer buffer(data, size);
    FieldHeader header;
    if (buffer.Size() < sizeof(header))
    {
        return;
    }

    buffer.ReadString((char*)&header, sizeof(header));
    if (header.field_type != INT64)
    {
        return;
    }

    if (buffer.Size() < header.field_len)
    {
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
    }
}

#pragma region 连接管理

void RpcClient::SubmitReq2Listener(bbt::network::ConnId id, emRpcConnType type, bbt::core::Buffer& buffer)
{
    ProtocolHead* head = (ProtocolHead*)buffer.Peek();

    switch (type)
    {
    case RPC_CONN_TYPE_CR:
        _R_Dispatch(id, buffer.Peek(), head->protocol_length);
        break;
    case RPC_CONN_TYPE_CS:
        _S_Dispatch(id, buffer.Peek(), head->protocol_length);
        break;
    default:
        OnError(util::errcode::Errcode{"[RpcClient::SubmitReq2Listener] unknown conn type", util::errcode::CommonErr});
        break;
    }
}

void RpcClient::NotifySend2Listener(bbt::network::ConnId id, emRpcConnType type, util::errcode::ErrOpt err, size_t len)
{

}

void RpcClient::NotityOnClose2Listener(bbt::network::ConnId id, emRpcConnType type)
{

}

void RpcClient::NotityOnTimeout2Listener(bbt::network::ConnId id, emRpcConnType type)
{

}

util::errcode::ErrOpt RpcClient::_SendToNode(bbt::network::ConnId connid, const bbt::core::Buffer& buffer)
{

}

util::errcode::ErrOpt RpcClient::_SendToRegistery(bbt::core::Buffer& buffer)
{
    auto conn = m_registery_client->GetConn();
    if (!conn || (conn && conn->IsClosed()))
        return util::errcode::Errcode{"[RpcClient::_SendToRegistery] conn is null", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.Size());
    return std::nullopt;
}

#pragma endregion

#pragma region 协议处理

util::errcode::ErrOpt RpcClient::_R_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    return std::nullopt;
}

util::errcode::ErrOpt RpcClient::_S_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len)
{
    return std::nullopt;
}


#pragma endregion

} // namespace plugin::ecs::rpc