#pragma once
#include <util/network/TcpClient.hpp>
#include <cluster/rpc/Define.hpp>
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

// FIXME 这里设计有问题，继承自RpcServer的RpcClient具备发送和接收的功能，应该是一个具体类
class RpcClient:
    public util::network::TcpClient
{
    friend class RpcServer;
public:
    RpcClient(std::shared_ptr<bbt::network::base::NetworkBase> network, const char* ip, short port, int timeout);
    virtual ~RpcClient();

    template<typename ...Args>
    int Call(RpcReplyCallback callback, const std::string& method, Args... args);

    virtual void OnReply(const char* data, size_t size) final;
protected:
    virtual int Send(const bbt::core::Buffer& buffer);
private:

    int64_t m_seq{0};
    std::unordered_map<int, RpcReplyCallback> m_callbacks;
};

template<typename ...Args>
int RpcClient::Call(RpcReplyCallback callback, const std::string& method, Args... args)
{
    RpcSerializer m_serializer;
    bbt::core::Buffer buffer = m_serializer.Serialize(method, ++m_seq, args...);

    m_callbacks[m_seq] = callback;
    return Send(buffer);
}

} // namespace plugin::ecs::rpc