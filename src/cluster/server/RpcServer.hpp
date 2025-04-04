#pragma once
#include <bbt/network/TcpServer.hpp>
#include <cluster/ClusterDefine.hpp>
#include <cluster/other/BufferMgr.hpp>

namespace cluster
{

struct ClientMgr
{
    std::unordered_map<bbt::network::ConnId, util::other::Uuid>
                                        m_rpc_client_connid_uuids; // 连接id到uuid的映射
    std::unordered_map<
        util::other::Uuid,
        std::shared_ptr<bbt::network::TcpClient>,
        util::other::Uuid::Hash>        m_rpc_clients;  // 与其他节点的连接
};

class RpcServer:
    public std::enable_shared_from_this<RpcServer>
{
    friend class RpcClient;
public:
    RpcServer(std::shared_ptr<bbt::network::EvThread> evthread);
    virtual ~RpcServer() noexcept;

    util::errcode::ErrOpt               Register(const std::string& method, RpcCallback callback);
    util::errcode::ErrOpt               OnRemoteCall(bbt::core::Buffer& req);
    std::vector<std::string>            GetRegistedMethods() const;
    bool                                HasMethod(const std::string& method) const;

    void                                Init(
        const IPAddress& listen_addr,
        const IPAddress& registery_addr);
    void                                SetConnectTimeout(int timeout) { m_connect_timeout = timeout; }
    void                                SetConnectionTimeout(int timeout){ m_connection_timeout = timeout; };
    util::errcode::ErrOpt               Start();
    void                                Stop();
    virtual void                        Update();

    virtual util::other::Uuid::SPtr     GetUUID() const;
    virtual const std::string&          GetName() const;

    virtual util::errcode::ErrOpt       OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply);


    virtual void                        OnError(const util::errcode::Errcode& err) = 0;
    virtual void                        OnInfo(const std::string& info) = 0;
    virtual void                        OnDebug(const std::string& info) = 0;

    virtual void                        OnCloseNode(bbt::network::ConnId id, const bbt::network::IPAddress& addr) {}

private:
    // util::errcode::ErrOpt               InitNetwork();
    util::errcode::ErrOpt               _InitRegisteryClient();
    util::errcode::ErrOpt               _InitTcpServer();
    void                                _DelayConnectToRegistery();
    void                                _ConnectToRegistery();
    void                                OnHandshakeSucc();

    void                                RequestFromRegistery(const bbt::core::Buffer& buffer);
    void                                OnTimeoutFromRegistey(bbt::network::ConnId id);
    void                                OnCloseFromRegistery(bbt::network::ConnId id, const IPAddress& addr);

    virtual util::errcode::ErrOpt       _SendToRegistery(protocol::emN2RProtocolType type, const bbt::core::Buffer& buffer);
    void                                R2S_OnConnect(bbt::network::ConnId id);
    void                                R2S_OnClose(bbt::network::ConnId id);
    void                                R2S_OnTimeout(bbt::network::ConnId id);
    void                                R2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                                R2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);

    virtual util::errcode::ErrOpt       _SendToClient(bbt::network::ConnId id, const bbt::core::Buffer& buffer);
    void                                C2S_OnAccept(bbt::network::ConnId id);
    void                                C2S_OnClose(bbt::network::ConnId id);
    void                                C2S_OnTimeout(bbt::network::ConnId id);
    void                                C2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                                C2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);

    std::shared_ptr<util::other::Uuid>  GetRandomUuidByMethod(const std::string& method);

    // n2n
    util::errcode::ErrOpt               C2S_Dispatch(bbt::network::ConnId id, protocol::emC2SProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt               C2S_OnCallRemoteMethod(bbt::network::ConnId id, protocol::C2S_CallMethod_Req* req);

    util::errcode::ErrOpt               R2N_Dispatch(protocol::emR2NProtocolType type, void* proto, size_t proto_len);

    // r2n response
    util::errcode::ErrOpt               R2S_OnHandshakeResp(protocol::R2N_Handshake_Resp* resp);
    util::errcode::ErrOpt               R2S_OnHeartBeatResp(protocol::R2N_KeepAlive_Resp* resp);
    util::errcode::ErrOpt               R2S_OnRegisterMethodResp(protocol::R2N_RegisterMethod_Resp* resp);

    // n2r request
    util::errcode::ErrOpt               S2R_DoHandshakeReq();
    util::errcode::ErrOpt               S2R_DoHeartBeatReq();
    util::errcode::ErrOpt               S2R_DoRegisterMethodReq(const std::string& method, util::other::Uuid signature);

private:
    std::mutex                           m_all_opt_mtx; // 保护所有操作的互斥锁

    std::unordered_map<std::string, RpcCallback> m_registed_methods;    // 注册的服务方法
    
    std::shared_ptr<bbt::network::EvThread> m_ev_thread{nullptr};
    std::string                         m_service_name{""};
    util::other::Uuid::SPtr             m_uuid{nullptr};
    IPAddress                           m_listen_addr;
    IPAddress                           m_registery_addr;

    std::shared_ptr<bbt::network::TcpServer>
                                        m_tcp_server{nullptr};  // 节点网络服务
    std::shared_ptr<bbt::network::TcpClient> m_registery_client{nullptr};    // 与注册中心的连接
    bool                                m_registery_connected{false};   // 是否完成连接

    ClientMgr                           m_client_conn_mgr;
    std::mutex                          m_client_conn_mgr_mtx;

    BufferMgr                           m_buffer_mgr;           // 缓存管理

    int                                 m_connect_timeout{BBGENGINE_CONNECT_TIMEOUT};
    int                                 m_heartbeat_timeout{BBGENGINE_HEARTBEAT_TIMEOUT_MS};
    int                                 m_reconnect_time{BBGENGINE_RECONNECT_TIME};
    int                                 m_connection_timeout{BBGENGINE_CLUSTER_CONN_FREE_TIMEOUT};
    bbt::core::clock::Timestamp<>       m_connect_to_registery_ms{bbt::core::clock::now()};
    bbt::core::clock::Timestamp<>       m_last_heatbeart_ms{bbt::core::clock::now()};
};

} // namespace cluster::rpc