#pragma once
#include <atomic>
#include <bbt/network/TcpClient.hpp>
#include <cluster/other/RpcSerializer.hpp>
#include <cluster/protocol/Protocol.hpp>
#include <cluster/client/ServiceMgr.hpp>

namespace cluster
{

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

    util::errcode::ErrOpt       C2S_SendRemoteCall(bbt::network::ConnId id, const std::string& method, const bbt::core::Buffer& params);

private:
    std::mutex                                          m_all_opt_mtx; // 所有操作的锁
    // 先支持单独的io线程
    std::shared_ptr<bbt::network::EvThread>             m_ev_thread{nullptr};
    bbt::network::IPAddress                             m_registery_addr;

    // rpc call
    RpcSerializer                                       m_serializer;
    std::atomic_int64_t                                 m_seq{0};
    std::unordered_map<int, RpcReplyCallback>           m_callbacks;

    // node info cache
    ServiceMgr                                          m_service_mgr;
    std::shared_ptr<bbt::network::TcpClient>            m_registery_client{nullptr};
    bbt::core::clock::Timestamp<>                       m_cache_last_update_time{bbt::core::clock::now()};
    const int                                           m_cache_update_interval{1000};  // 1s 更新一次

    int                                                 m_reconnect_time{BBGENGINE_RECONNECT_TIME};
    bbt::core::clock::Timestamp<>                       m_connect_to_registery_ms{bbt::core::clock::now()};
};

template<typename ...Args>
util::errcode::ErrOpt RpcClient::DoRemoteCall(RpcReplyCallback callback, const std::string& method, Args... args)
{
    // 获取调用节点
    std::lock_guard<std::mutex> lock(m_all_opt_mtx);
    bbt::core::Buffer buffer = m_serializer.Serialize(method, ++m_seq, args...);

    Assert(callback);
    m_callbacks[m_seq] = callback;

    auto connid = GetServiceConn(method);
    if (connid <= 0)
        return util::errcode::Errcode{"no service!", util::errcode::emErr::CommonErr};

    if (auto err = C2S_SendRemoteCall(connid, method, buffer); err != std::nullopt)
        return err;

    return std::nullopt;
}

} // namespace plugin::ecs::rpc