#pragma once
#include <plugin/ecs/rpc/Define.hpp>
#include <plugin/ecs/rpc/RpcSerializer.hpp>

namespace plugin::ecs::rpc
{

class RpcClient:
    public engine::ecs::Component
{
public:
    RpcClient(engine::ecs::ComponentTemplateId id);
    ~RpcClient();

    template<typename ...Args>
    int Call(RpcReplyCallback callback, const std::string& method, Args... args);

protected:
    virtual int Send(const bbt::buffer::Buffer& buffer) = 0;
private:
    virtual void OnRecv(const char* data, size_t size) final;

    int64_t m_seq{0};
    std::unordered_map<int, RpcReplyCallback> m_callbacks;
};

template<typename ...Args>
int RpcClient::Call(RpcReplyCallback callback, const std::string& method, Args... args)
{
    RpcSerializer m_serializer;
    bbt::buffer::Buffer buffer = m_serializer.Serialize(method, ++m_seq, args...);

    m_callbacks[m_seq] = callback;
    return Send(buffer);
}

} // namespace plugin::ecs::rpc