#pragma once
#include <bbt/network/TcpServer.hpp>
#include <cluster/rpc/Define.hpp>

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

    int                                 Register(const std::string& method, RpcCallback callback);
    util::errcode::ErrOpt               OnRemoteCall(bbt::core::Buffer& req);
    std::vector<std::string>            GetRegistedMethods() const;
    bool                                HasMethod(const std::string& method) const;

    void                                Init(const IPAddress& listen_addr, const IPAddress& registery_addr, int timeout);
    util::errcode::ErrOpt               Start();
    void                                Stop();
    virtual void                        Update();
    virtual void                        Offline();
    virtual void                        Online();
    virtual void                        Clear();

    virtual util::other::Uuid::SPtr     GetUUID() const;
    virtual NodeState                   GetNodeState();
    virtual const std::string&          GetName() const;

    virtual util::errcode::ErrOpt       RemoteCall(const std::string& method, bbt::core::Buffer& buffer);
    virtual util::errcode::ErrOpt       OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply);

    virtual util::errcode::ErrOpt       SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    void                                OnSendToRegistery(util::errcode::ErrOpt err, size_t len);
    virtual util::errcode::ErrOpt       SendToRegistery(protocol::emN2RProtocolType type, const bbt::core::Buffer& buffer);

    virtual void                        OnError(const util::errcode::Errcode& err) = 0;
    virtual void                        OnInfo(const std::string& info) = 0;
    virtual void                        OnDebug(const std::string& info) = 0;

    virtual void                        OnCloseNode(bbt::network::ConnId id, const bbt::network::IPAddress& addr) {}

    // 监听连接事件
    void                                SubmitReq2Listener(bbt::network::ConnId id, bbt::core::Buffer& buffer);        // 从连接中获取请求，提交给监听者处理
    void                                NotifySend2Listener(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len); // 通知监听者发送结果
    void                                NotityOnClose2Listener(bbt::network::ConnId id); // 通知监听者连接关闭
    void                                NotityOnTimeout2Listener(bbt::network::ConnId id); // 通知监听者连接超时
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

    void                                R2S_OnConnect(bbt::network::ConnId id);
    void                                R2S_OnClose(bbt::network::ConnId id);
    void                                R2S_OnTimeout(bbt::network::ConnId id);
    void                                R2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                                R2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);

    void                                C2S_OnAccept(bbt::network::ConnId id);
    void                                C2S_OnClose(bbt::network::ConnId id);
    void                                C2S_OnTimeout(bbt::network::ConnId id);
    void                                C2S_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                                C2S_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);

    std::shared_ptr<util::other::Uuid>  GetRandomUuidByMethod(const std::string& method);

    // n2n
    util::errcode::ErrOpt               N2N_Dispatch(bbt::network::ConnId id, protocol::emN2NProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt               N2N_OnCallRemoteMethod(bbt::network::ConnId id, protocol::N2N_CallMethod_Req* req);

    util::errcode::ErrOpt               R2N_Dispatch(protocol::emR2NProtocolType type, void* proto, size_t proto_len);

    // r2n response
    util::errcode::ErrOpt               R2N_OnHandshakeResp(protocol::R2N_Handshake_Resp* resp);
    util::errcode::ErrOpt               R2N_OnHeartBeatResp(protocol::R2N_KeepAlive_Resp* resp);
    util::errcode::ErrOpt               R2N_OnRegisterMethodResp(protocol::R2N_RegisterMethod_Resp* resp);

    // n2r request
    util::errcode::ErrOpt               N2R_DoHandshakeReq();
    util::errcode::ErrOpt               N2R_DoHeartBeatReq();
    util::errcode::ErrOpt               N2R_DoRegisterMethodReq(const std::string& method, util::other::Uuid signature);
private:
    std::unordered_map<std::string, RpcCallback> m_registed_methods;    // 注册的服务方法
    
    std::shared_ptr<bbt::network::EvThread> m_ev_thread{nullptr};
    NodeState                           m_state{NODESTATE_UNREGISTER};
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

    int                                 m_connect_timeout{20000};
    int                                 m_heartbeat_timeout{3000};
    int                                 m_reconnect_time{3000};
    bbt::core::clock::Timestamp<>       m_connect_to_registery_ms{bbt::core::clock::now()};
    bbt::core::clock::Timestamp<>       m_last_heatbeart_ms{bbt::core::clock::now()};
};

} // namespace cluster::rpc