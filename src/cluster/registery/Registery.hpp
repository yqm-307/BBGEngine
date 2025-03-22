#pragma once
#include <bbt/network/TcpServer.hpp>
#include <cluster/registery/NodeMgr.hpp>
#include <cluster/protocol/Protocol.hpp>

namespace cluster
{


class Registery:
    public std::enable_shared_from_this<Registery>
{
    friend class R2NConnection;
public:
    explicit Registery();
    virtual ~Registery();

    void                            Init(
                                        const bbt::network::IPAddress& rs_listen,
                                        const bbt::network::IPAddress& rc_listen,
                                        std::shared_ptr<bbt::network::TcpServer> rs_server,
                                        std::shared_ptr<bbt::network::TcpServer> rc_server,
                                        int timeout_ms);
    void                            Update();
    void                            Start();
    void                            Stop();

    util::errcode::ErrOpt           SendToNode(protocol::emR2NProtocolType type, const util::other::Uuid& uuid, const bbt::core::Buffer& buffer);
    util::errcode::ErrOpt           SendToNode(protocol::emR2NProtocolType type, const bbt::network::ConnId& connid, const bbt::core::Buffer& buffer);
    void                            OnSendToNode(util::errcode::ErrOpt err, size_t len);
    void                            OnRequest(bbt::network::ConnId id, bbt::core::Buffer& buffer);

    virtual void                    OnError(const util::errcode::Errcode& err) = 0;
    virtual void                    OnInfo(const std::string& info) = 0;
    virtual void                    OnDebug(const std::string& info) = 0;

    NodeRegInfo::SPtr               GetNodeRegInfo(const util::other::Uuid& uuid);
    void                            CloseConn(bbt::network::ConnId connid);
    NodeState                       GetNodeStatus(const util::other::Uuid& uuid) const;

    // 节点管理
    void                            RegisterNode(const bbt::network::IPAddress& addr, const util::other::Uuid& uuid);
    void                            UnRegisterNode(const util::other::Uuid& uuid);

    // RpcClient 网络事件
    // void                            SubmitReq2Listener(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    void                            RC_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                            RC_OnClose(bbt::network::ConnId id);
    void                            RC_OnTimeout(bbt::network::ConnId id);
    void                            RC_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);
    void                            NotifyOnAccept(bbt::network::ConnId connid);

    // RpcServer 网络事件
    void                            RS_OnSend(bbt::network::ConnId id, util::errcode::ErrOpt err, size_t len);
    void                            RS_OnClose(bbt::network::ConnId id);
    void                            RS_OnTimeout(bbt::network::ConnId id);
    void                            RS_OnRecv(bbt::network::ConnId id, const bbt::core::Buffer& buffer);
private:
    void                            _InitRCServer();
    void                            _InitRSServer();
    // Node握手相关
    bool                            IsHalfConn(bbt::network::ConnId connid);
    void                            DelHalfConn(bbt::network::ConnId connid);
    void                            AddHalfConn(bbt::network::ConnId connid);

    bbt::network::ConnId            GetConnIdByUuid(const util::other::Uuid& uuid) const;
    // 与节点的网络事件
    util::errcode::ErrOpt           S2RDispatch(bbt::network::ConnId id, protocol::emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_KeepAlive_Req* req);
    util::errcode::ErrOpt           OnHandshake(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_Handshake_Req* req);
    util::errcode::ErrOpt           OnRegisterMethod(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_RegisterMethod_Req* req);

    util::errcode::ErrOpt           C2RDispatch(bbt::network::ConnId id, protocol::emC2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnGetNodesInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::C2R_GetNodesInfo_Req* req);
private:
    std::shared_ptr<bbt::network::EvThread>     m_ev_thread{nullptr};
    std::shared_ptr<bbt::network::TcpServer>    m_rs_server{nullptr};   // RpcServer监听
    IPAddress                                   m_rs_listen_addr;
    std::shared_ptr<bbt::network::TcpServer>    m_rc_server{nullptr};   // RpcClient监听
    IPAddress                                   m_rc_listen_addr;

    // 握手管理
    std::unordered_set<bbt::network::ConnId>    m_half_connect_set;
    std::mutex                                  m_half_connect_set_lock;
    
    std::shared_ptr<NodeMgr>                    m_node_mgr{nullptr};
};

} // namespace cluster