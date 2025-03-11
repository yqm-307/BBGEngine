#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/registery/NodeMgr.hpp>
#include <cluster/protocol/Protocol.hpp>
#include <cluster/connection/RpcConnection.hpp>

namespace cluster
{

class RegisteryServer;

class Registery:
    public std::enable_shared_from_this<Registery>
{
    friend class RegisteryServer;
    friend class R2NConnection;
public:
    explicit Registery();
    virtual ~Registery();

    void                            Init(const util::network::IPAddress& listen_addr, int timeout_ms);
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
    void                            RegisterNode(const util::network::IPAddress& addr, const util::other::Uuid& uuid);
    void                            UnRegisterNode(const util::other::Uuid& uuid);

    // 监听连接事件
    void                            SubmitReq2Listener(bbt::network::ConnId id, emRpcConnType type, bbt::core::Buffer& buffer); // 从连接中获取请求，提交给监听者处理
    void                            NotifySend2Listener(bbt::network::ConnId id, emRpcConnType type, util::errcode::ErrOpt err, size_t len); // 通知监听者发送结果
    void                            NotityOnClose2Listener(bbt::network::ConnId id, emRpcConnType type); // 通知监听者连接关闭
    void                            NotityOnTimeout2Listener(bbt::network::ConnId id, emRpcConnType type); // 通知监听者连接超时
    void                            NotifyOnAccept(bbt::network::ConnId connid);
private:
    // Node握手相关
    bool                            IsHalfConn(bbt::network::ConnId connid);
    void                            DelHalfConn(bbt::network::ConnId connid);
    void                            AddHalfConn(bbt::network::ConnId connid);

    bbt::network::ConnId            GetConnIdByUuid(const util::other::Uuid& uuid) const;
    // 与节点的网络事件
    util::errcode::ErrOpt           N2RDispatch(bbt::network::ConnId id, protocol::emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_KeepAlive_Req* req);
    util::errcode::ErrOpt           OnHandshake(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_Handshake_Req* req);
    util::errcode::ErrOpt           OnRegisterMethod(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_RegisterMethod_Req* req);
    util::errcode::ErrOpt           OnGetNodesInfo(bbt::network::ConnId id, protocol::ProtocolHead* head, protocol::N2R_GetNodesInfo_Req* req);
private:
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};
    std::shared_ptr<util::network::TcpServer>   m_registery_server{nullptr};

    // 握手管理
    std::unordered_set<bbt::network::ConnId>    m_half_connect_set;
    std::mutex                                  m_half_connect_set_lock;
    
    std::shared_ptr<RegisteryServer>            m_service{nullptr};
    std::shared_ptr<NodeMgr>                    m_node_mgr{nullptr};
};

} // namespace cluster