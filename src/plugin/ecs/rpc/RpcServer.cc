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
    FieldValue field;
    std::string method;
    int64_t call_seq = 0;
    RpcSerializer coder;

    // 读取 mothod name
    {
        auto err = coder.DeserializeOne(buffer, field);
        if (err)
            return err;
        
        if (field.header.field_type != STRING)
            return util::errcode::ErrCode("a bad protocol! method name must be string", util::errcode::ErrType::CommonErr);

        method = std::move(field.string);
    }

    // 读取 call seq
    {
        auto err = coder.DeserializeOne(buffer, field);
        if (err) 
            return err;

        if (field.header.field_type != INT64)
            return util::errcode::ErrCode("a bad protocol! method call_seq must be string", util::errcode::ErrType::CommonErr);

        call_seq = field.value.int64_value;
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