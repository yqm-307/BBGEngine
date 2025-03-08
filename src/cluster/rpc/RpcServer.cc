#include <cluster/rpc/RpcServer.hpp>
#include <cluster/rpc/RpcSerializer.hpp>
#include <cluster/node/ClusterNode.hpp>

namespace cluster
{

RpcServer::RpcServer(std::shared_ptr<ClusterNode> node, const util::network::IPAddress& listen_addr, int timeout):
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

util::errcode::ErrOpt RpcServer::OnRemoteCall(bbt::core::Buffer& req)
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
            return util::errcode::Errcode("a bad protocol! method name must be string", util::errcode::emErr::CommonErr);

        method = std::move(field.string);
    }

    // 读取 call seq
    {
        auto err = coder.DeserializeOne(req, field);
        if (err) 
            return err;

        if (field.header.field_type != INT64)
            return util::errcode::Errcode("a bad protocol! method call_seq must be string", util::errcode::emErr::CommonErr);

        call_seq = field.value.int64_value;
    }
    

    auto iter = m_registed_methods.find(method);
    if (iter == m_registed_methods.end())
        return util::errcode::Errcode("method not found", util::errcode::emErr::CommonErr);

    return std::nullopt;
}

std::vector<std::string> RpcServer::GetRegistedMethods() const
{
    std::vector<std::string> methods;
    for (auto& iter : m_registed_methods) {
        methods.push_back(iter.first);
    }

    return methods;
}

bool RpcServer::HasMethod(const std::string& method) const
{
    return m_registed_methods.find(method) != m_registed_methods.end();
}

} // namespace cluster::rpc