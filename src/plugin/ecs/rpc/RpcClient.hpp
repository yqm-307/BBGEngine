#pragma once
#include <plugin/ecs/rpc/Define.hpp>
#include <plugin/ecs/rpc/RpcSerializer.hpp>

namespace plugin::ecs::rpc
{

class RpcClient:
    public engine::ecs::Component
{
    friend class RpcServer;
public:
    RpcClient(engine::ecs::ComponentTemplateId id);
    ~RpcClient();

    template<typename ...Args>
    int Call(RpcReplyCallback callback, const std::string& method, Args... args);

    virtual void OnReply(const char* data, size_t size) final;
protected:
    virtual int Send(const bbt::core::Buffer& buffer) = 0;
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