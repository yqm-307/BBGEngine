#include <cluster/rpc/RpcServer.hpp>
#include <cluster/rpc/RpcSerializer.hpp>
#include <cluster/node/ClusterNodeBase.hpp>

namespace cluster
{

RpcServer::RpcServer(std::shared_ptr<ClusterNodeBase> node, const bbt::net::IPAddress& listen_addr, int timeout):
    util::network::TcpServer(listen_addr.GetIP(), listen_addr.GetPort(), timeout),
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

util::errcode::ErrOpt RpcServer::OnRpc(bbt::core::Buffer& buffer)
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

    bbt::core::Buffer resp = coder.Serialize(call_seq);

    auto err = iter->second(buffer, resp);
    if (err == std::nullopt) {
        Send(resp.Peek(), resp.DataSize());
    }

    return err;
}

std::shared_ptr<util::network::Connection> RpcServer::CreateConnection(bbt::network::libevent::ConnectionSPtr conn)
{
    auto rpc_conn = std::make_shared<NNConnection>(conn, 5000);
    auto connid = rpc_conn->GetConnId();

    rpc_conn->SetCallbacks({
        .on_recv_callback = [weak_this{weak_from_this()}, connid](auto, const char* data, size_t len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                bbt::core::Buffer buffer{data, len};
                if (auto err = std::static_pointer_cast<RpcServer>(shared_this)->OnRpc(buffer); err != std::nullopt) {
                    if (auto cluster_node = std::static_pointer_cast<RpcServer>(shared_this)->m_node_weak.lock(); cluster_node != nullptr) {
                        cluster_node->OnError(err.value());
                    }
                }
            }
        },
        .on_send_callback = [weak_this{weak_from_this()}, connid](auto, const bbt::errcode::Errcode& err, size_t succ_len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto cluster_node = std::static_pointer_cast<RpcServer>(shared_this)->m_node_weak.lock(); cluster_node != nullptr) {
                    if (err.IsErr())
                        cluster_node->OnError(err);
                }
            }
        },
        .on_close_callback = [weak_this{weak_from_this()}, connid](auto, auto)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                shared_this->DelConnect(connid);
            }
        },
        .on_timeout_callback = [weak_this{weak_from_this()}, connid](auto)
        {
            // 这里超时后，回继续调用OnClose，所以在onclose中删除连接
        },
        .on_err_callback = [weak_this{weak_from_this()}, connid](auto, const bbt::errcode::Errcode& err)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto cluster_node = std::static_pointer_cast<RpcServer>(shared_this)->m_node_weak.lock(); cluster_node != nullptr) {
                    cluster_node->OnError(err);
                }
            }
        },
    });

    return rpc_conn;
}

} // namespace cluster::rpc