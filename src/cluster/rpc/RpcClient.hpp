#pragma once
#include <atomic>
#include <util/network/TcpClient.hpp>
#include <cluster/connection/RpcConnection.hpp>
#include <cluster/protocol/Protocol.hpp>
#include <cluster/rpc/Define.hpp>
#include <cluster/rpc/RpcSerializer.hpp>

namespace cluster
{

class NodeCache
{
public:
    NodeCache() = default;
    virtual ~NodeCache() = default;

    void UpdateCache(const util::other::Uuid& uuid, const std::vector<RpcMethodHash>& methods);
    void DeleteCache(const util::other::Uuid& uuid);

private:
    std::unordered_map<RpcMethodHash, std::unordered_set<util::other::Uuid, util::other::Uuid::Hash>> m_method_uuids;
    std::unordered_map<util::other::Uuid, std::unordered_set<RpcMethodHash>, util::other::Uuid::Hash> m_node_methods;
};


class RpcClient:
    public std::enable_shared_from_this<RpcClient>
{
    friend class RpcServer;
public:
    RpcClient();
    virtual ~RpcClient();

    util::errcode::ErrOpt       Init(const util::network::IPAddress& registery_addr, int timeout_ms);
    // 去Registery请求数据
    util::errcode::ErrOpt       RequestNodeInfo(const util::network::IPAddress& registery_addr, std::function<void()> notify_cb);
    // const util::network::IPAddress& GetServiceAddr(const std::string& method);  // 获取服务地址
    // util::other::Uuid           GetServiceUuid(const std::string& method);       // 获取服务uuid
    std::shared_ptr<util::network::TcpClient> GetServiceConn(const std::string& method); // 获取服务连接

    virtual void                OnReply(const char* data, size_t size) final;

    virtual void                OnError(const util::errcode::Errcode& err) = 0;
    virtual void                OnInfo(const std::string& info) = 0;
    virtual void                OnDebug(const std::string& info) = 0;

    void                        SubmitReq2Listener(bbt::network::ConnId id, emRpcConnType type, bbt::core::Buffer& buffer);        // 从连接中获取请求，提交给监听者处理
    void                        NotifySend2Listener(bbt::network::ConnId id, emRpcConnType type, util::errcode::ErrOpt err, size_t len); // 通知监听者发送结果
    void                        NotityOnClose2Listener(bbt::network::ConnId id, emRpcConnType type); // 通知监听者连接关闭
    void                        NotityOnTimeout2Listener(bbt::network::ConnId id, emRpcConnType type); // 通知监听者连接超时
private:
    util::errcode::ErrOpt       _SendToNode(bbt::network::ConnId connid, const bbt::core::Buffer& buffer);
    util::errcode::ErrOpt       _SendToRegistery(protocol::emC2RProtocolType type, const bbt::core::Buffer& buffer);

    util::errcode::ErrOpt       _R_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len);
    util::errcode::ErrOpt       DoGetNodesInfoReq();
    util::errcode::ErrOpt       _OnGetNodeInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::R2C_GetNodesInfo_Resp* req);

    util::errcode::ErrOpt       _S_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len);
    template<typename ...Args>
    util::errcode::ErrOpt       DoRemoteCall(RpcReplyCallback callback, const std::string& method, Args... args);

private:
    std::shared_ptr<bbt::network::base::NetworkBase>    m_network{nullptr};

    // 链接管理
    // ConnMgr                                             m_conn_mgr;
    std::mutex                                          m_conn_mgr_mtx;
    std::shared_ptr<util::network::TcpClient>           m_registery_client{nullptr};

    // rpc call
    RpcSerializer                                       m_serializer;
    std::atomic_int64_t                                 m_seq{0};
    std::unordered_map<int, RpcReplyCallback>           m_callbacks;
    std::mutex                                          m_reply_mtx;

    // node info cache
    NodeCache                                           m_node_cache;
    std::mutex                                          m_node_cache_mtx;
};

template<typename ...Args>
util::errcode::ErrOpt RpcClient::DoRemoteCall(RpcReplyCallback callback, const std::string& method, Args... args)
{
    // 获取调用节点

    std::lock_guard<std::mutex> lock(m_reply_mtx);
    bbt::core::Buffer buffer = m_serializer.Serialize(method, ++m_seq, args...);

    m_callbacks[m_seq] = callback;

    return std::nullopt;
}

} // namespace plugin::ecs::rpc