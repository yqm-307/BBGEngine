#include <plugin/ecs/rpc/RpcServer.hpp>
#include <plugin/ecs/rpc/RpcSerializer.hpp>

namespace plugin::ecs::rpc
{

RpcServer::RpcServer(engine::ecs::ComponentTemplateId id):
    Component(id)
{
}

RpcServer::~RpcServer()
{
}

int RpcServer::Register(const std::string& method, RpcCallback callback)
{
    m_registed_methods[method] = callback;
    return 0;
}

util::errcode::ErrOpt RpcServer::OnRpc(bbt::buffer::Buffer& buffer)
{
    FieldHeader header;
    std::string method;
    int64_t call_seq = 0;
    RpcSerializer coder;

    // 读取 mothod name
    {
        if (buffer.ReadableBytes() < sizeof(header))
            return util::errcode::ErrCode("a bad protocol! too short", util::errcode::ErrType::CommonErr);

        buffer.ReadString((char*)&header, sizeof(header));
        if (header.field_type != STRING)
            return util::errcode::ErrCode("a bad protocol! method name must be string", util::errcode::ErrType::CommonErr);

        if (buffer.ReadableBytes() < header.field_len)
            return util::errcode::ErrCode("a bad protocol! method name too long", util::errcode::ErrType::CommonErr);

        method.resize(header.field_len);
        buffer.ReadString(method.data(), method.size());

    }

    // 读取 call seq
    {
        if (buffer.ReadableBytes() < sizeof(header))
            return util::errcode::ErrCode("a bad protocol! too short", util::errcode::ErrType::CommonErr);

        buffer.ReadString((char*)&header, sizeof(header));
        if (header.field_type != INT64)
            return util::errcode::ErrCode("a bad protocol! method call_seq must be string", util::errcode::ErrType::CommonErr);
        
        if (buffer.ReadableBytes() < header.field_len)
            return util::errcode::ErrCode("a bad protocol! method name too long", util::errcode::ErrType::CommonErr);

        call_seq = buffer.ReadInt64();
    }
    

    auto iter = m_registed_methods.find(method);
    if (iter == m_registed_methods.end())
        return util::errcode::ErrCode("method not found", util::errcode::ErrType::CommonErr);

    bbt::buffer::Buffer resp = coder.Serialize(call_seq);

    auto err = iter->second(buffer, resp);
    if (err == std::nullopt) {
        Send(resp.Peek(), resp.DataSize());
    }

    return err;
}

} // namespace plugin::ecs::rpc