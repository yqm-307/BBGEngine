#pragma once
#include <atomic>
#include <bbt/network/TcpClient.hpp>
#include <cluster/protocol/Protocol.hpp>
#include <cluster/other/RpcSerializer.hpp>

namespace cluster
{

class NodeCache
{
public:
    NodeCache() = default;
    virtual ~NodeCache() = default;

    void UpdateCache(const util::other::Uuid& uuid, const std::vector<RpcMethodHash>& methods);
    void DeleteCache(const util::other::Uuid& uuid);

    std::optional<util::other::Uuid> GetUuid(RpcMethodHash method);
private:
    std::unordered_map<RpcMethodHash, std::unordered_set<util::other::Uuid, util::other::Uuid::Hash>> m_method_uuids;
    std::unordered_map<util::other::Uuid, std::unordered_set<RpcMethodHash>, util::other::Uuid::Hash> m_node_methods;
};

struct ServiceMgr
{
    std::unordered_map<bbt::network::ConnId, util::other::Uuid>
                                        m_rpc_client_connid_uuids; // 连接id到uuid的映射
    std::unordered_map<
        util::other::Uuid,
        std::shared_ptr<bbt::network::TcpClient>,
        util::other::Uuid::Hash>        m_rpc_clients;  // 与其他节点的连接
};

class RpcClient:
    public std::enable_shared_from_this<RpcClient>
{
    friend class RpcServer;
public:
    RpcClient(std::shared_ptr<bbt::network::EvThread> evthread);
    virtual ~RpcClient();

    util::errcode::ErrOpt       Init(const bbt::core::net::IPAddress& registery_addr, int timeout_ms);

    void                        Start();
    void                        Stop();
    void                        Update();

    // const bbt::network::IPAddress& GetServiceAddr(const std::string& method);  // 获取服务地址
    // util::other::Uuid           GetServiceUuid(const std::string& method);       // 获取服务uuid
    bbt::network::ConnId        GetServiceConn(const std::string& method); // 获取服务连接

    virtual void                OnReply(const char* data, size_t size) final;
    template<typename ...Args>
    util::errcode::ErrOpt       DoRemoteCall(RpcReplyCallback callback, const std::string& method, Args... args);

    virtual void                OnError(const util::errcode::Errcode& err) = 0;
    virtual void                OnInfo(const std::string& info) = 0;
    virtual void                OnDebug(const std::string& info) = 0;

    util::errcode::ErrOpt       DoGetNodesInfo();
private:
    // registery 网络事件
    util::errcode::ErrOpt       _InitRegisteryClient();
    void                        _DelayConnectToRegistery();
    util::errcode::ErrOpt       _ConnectToRegistery();
    util::errcode::ErrOpt       _SendToRegistery(protocol::emC2RProtocolType type, const bbt::core::Buffer& buffer);
    void                        R2C_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                        R2C_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);
    void                        R2C_OnClose(bbt::network::ConnId id);
    void                        R2C_OnTimeout(bbt::network::ConnId id);
    void                        R2C_OnConnect(bbt::network::ConnId id);
    
    // rpc server 网络事件
    util::errcode::ErrOpt       _SendToServer(bbt::network::ConnId connid, const bbt::core::Buffer& buffer);
    util::errcode::ErrOpt       R2C_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len);
    util::errcode::ErrOpt       R2C_OnGetNodeInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::R2C_GetNodesInfo_Resp* resq);
    
    util::errcode::ErrOpt       S2C_Dispatch(bbt::network::ConnId connid, const char* proto, size_t len);
    util::errcode::ErrOpt       S2C_OnRemoteCall(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::S2C_CallMethod_Resp* resq);

private:
    // 先支持单独的io线程
    std::shared_ptr<bbt::network::EvThread>             m_ev_thread{nullptr};
    bbt::network::IPAddress                             m_registery_addr;

    // rpc call
    RpcSerializer                                       m_serializer;
    std::atomic_int64_t                                 m_seq{0};
    std::unordered_map<int, RpcReplyCallback>           m_callbacks;
    std::mutex                                          m_reply_mtx;

    // node info cache
    ServiceMgr                                           m_server_mgr;
    NodeCache                                           m_node_cache;
    std::shared_ptr<bbt::network::TcpClient>            m_registery_client{nullptr};
    bbt::core::clock::Timestamp<>                       m_cache_last_update_time{bbt::core::clock::now()};
    const int                                           m_cache_update_interval{1000};  // 1s 更新一次
    std::mutex                                          m_net_mtx;

    int                                                 m_reconnect_time{BBGENGINE_RECONNECT_TIME};
    bbt::core::clock::Timestamp<>                       m_connect_to_registery_ms{bbt::core::clock::now()};
};

template<typename ...Args>
util::errcode::ErrOpt RpcClient::DoRemoteCall(RpcReplyCallback callback, const std::string& method, Args... args)
{
    // 获取调用节点
    std::lock_guard<std::mutex> lock(m_reply_mtx);
    bbt::core::Buffer buffer = m_serializer.Serialize(method, ++m_seq, args...);

    m_callbacks[m_seq] = callback;

    auto connid = GetServiceConn(method);

    if (auto err = _SendToServer(connid, buffer); err != std::nullopt)
        return err;

    return std::nullopt;
}

} // namespace plugin::ecs::rpc