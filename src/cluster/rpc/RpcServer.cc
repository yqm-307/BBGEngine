#include <cluster/rpc/RpcServer.hpp>
#include <cluster/rpc/RpcSerializer.hpp>
#include <cluster/node/ClusterNode.hpp>

namespace cluster
{

RpcServer::RpcServer(std::shared_ptr<ClusterNode> node, const bbt::net::IPAddress& listen_addr, int timeout):
    m_node_weak(node)
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

bbt::errcode::ErrOpt RpcServer::OnRemoteCall(bbt::core::Buffer& req, bbt::core::Buffer& reply)
{
    FieldValue field;
    std::string method;
    int64_t call_seq = 0;
    RpcSerializer coder;

    // 读取 mothod name
    {
        auto err = coder.DeserializeOne(req, field);
        if (err)
            return err;
        
        if (field.header.field_type != STRING)
            return bbt::errcode::Errcode("a bad protocol! method name must be string", util::errcode::emErr::CommonErr);

        method = std::move(field.string);
    }

    // 读取 call seq
    {
        auto err = coder.DeserializeOne(req, field);
        if (err) 
            return err;

        if (field.header.field_type != INT64)
            return bbt::errcode::Errcode("a bad protocol! method call_seq must be string", util::errcode::emErr::CommonErr);

        call_seq = field.value.int64_value;
    }
    

    auto iter = m_registed_methods.find(method);
    if (iter == m_registed_methods.end())
        return bbt::errcode::Errcode("method not found", util::errcode::emErr::CommonErr);

    reply.Write(call_seq);

    auto err = iter->second(req, reply);
    if (err == std::nullopt) {
        // Send(connid, resp.Peek(), resp.DataSize());
    }

    return err;
}

} // namespace cluster::rpc