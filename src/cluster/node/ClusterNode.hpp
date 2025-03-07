#pragma once
#include <util/other/Uuid.hpp>
#include <util/network/Connection.hpp>
#include <util/network/TcpServer.hpp>
#include <bbt/core/net/IPAddress.hpp>
#include <cluster/ClusterDefine.hpp>
#include <cluster/rpc/RpcClient.hpp>
#include <cluster/rpc/RpcServer.hpp>
#include <cluster/protocol/N2NProtocol.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <cluster/protocol/R2NProtocol.hpp>

namespace cluster
{

/**
 * @brief 集群节点基类
 * 负责维护节点状态，节点间通信，节点注册中心通信
 * 
 * 基类负责实现节点间通信行为，注册中心通信行为；派生类负责实现网络通信
 * 
 */
class ClusterNode:
    public boost::noncopyable,
    public std::enable_shared_from_this<ClusterNode>
{
public:
    ClusterNode();
    virtual ~ClusterNode();

    void                                Init(const util::network::IPAddress& listen_addr, const util::network::IPAddress& registery_addr, int timeout);
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

    // 基类定义节点间网络传输行为
    virtual util::errcode::ErrOpt       SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    void                                OnSendToRegistery(util::errcode::ErrOpt err, size_t len);
    // 基类定义注册中心网络传输行为
    virtual util::errcode::ErrOpt       SendToRegistery(bbt::core::Buffer& buffer);
    virtual void                        OnError(const util::errcode::Errcode& err) = 0;
    virtual void                        OnInfo(const std::string& info) = 0;
    virtual void                        OnDebug(const std::string& info) = 0;

    virtual void                        OnCloseNode(bbt::network::ConnId id, const util::network::IPAddress& addr) {}

    // 注册中心连接事件
    void                                RequestFromRegistery(bbt::core::Buffer& buffer);
    void                                OnTimeoutFromRegistey(bbt::network::ConnId id);
    void                                OnCloseFromRegistery(bbt::network::ConnId id, const util::network::IPAddress& addr);

    // 节点间连接事件
    void                                RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    void                                OnTimeoutFromNode(bbt::network::ConnId id);
    void                                OnCloseFromNode(bbt::network::ConnId id);

private:
    util::errcode::ErrOpt               InitNetwork();
    void                                _DelayConnectToRegistery();
    void                                _ConnectToRegistery();
    void                                OnHandshakeSucc();

    std::shared_ptr<util::other::Uuid>  GetRandomUuidByMethod(const std::string& method);

    util::errcode::ErrOpt               ProcessN2NProtocol();
    util::errcode::ErrOpt               ProcessN2RProtocol();

    // n2n
    util::errcode::ErrOpt               N2N_Dispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt               N2N_DoHeartBeat(bbt::network::ConnId id);

    util::errcode::ErrOpt               R2N_Dispatch(emR2NProtocolType type, void* proto, size_t proto_len);

    // r2n response
    util::errcode::ErrOpt               R2N_OnHandshakeResp(R2N_Handshake_Resp* resp);
    util::errcode::ErrOpt               R2N_OnHeartBeatResp(R2N_KeepAlive_Resp* resp);

    // n2r request
    util::errcode::ErrOpt               N2R_DoHandshakeReq();
    util::errcode::ErrOpt               N2R_DoHeatBeatReq();
    util::errcode::ErrOpt               N2R_DoRegisterMethodReq(const std::string& method, util::other::Uuid signature);
    util::errcode::ErrOpt               N2R_DoGetNodesInfoReq();

private:
    NodeState                           m_state{NODESTATE_UNREGISTER};
    std::string                         m_service_name{""};
    util::other::Uuid::SPtr             m_uuid{nullptr};
    util::network::IPAddress            m_listen_addr;
    util::network::IPAddress            m_registery_addr;
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};

    std::shared_ptr<RpcServer>          m_rpc_server{nullptr};  // 本地rpc服务
    std::shared_ptr<util::network::TcpServer>
                                        m_tcp_server{nullptr};  // 节点网络服务
    std::shared_ptr<util::network::TcpClient> m_registery_client{nullptr};    // 与注册中心的连接
    bool                                m_registery_connected{false};   // 是否完成连接
    std::unordered_map<bbt::network::ConnId, util::other::Uuid>
                                        m_rpc_client_connid_uuids; // 连接id到uuid的映射
    std::unordered_map<
        util::other::Uuid,
        std::shared_ptr<RpcClient>,
        util::other::Uuid::Hash>        m_rpc_clients;  // 与其他节点的连接

    int                                 m_connect_timeout{20000};
    int                                 m_heartbeat_timeout{3000};
    int                                 m_reconnect_time{3000};
    bbt::core::clock::Timestamp<>       m_connect_to_registery_ms{bbt::core::clock::now()};
    bbt::core::clock::Timestamp<>       m_last_heatbeart_ms{bbt::core::clock::now()};
    bbt::core::clock::Timestamp<>       m_last_get_nodes_info_ms{bbt::core::clock::now()};

    // 本地缓存
    std::unordered_map<std::string, std::unordered_set<util::other::Uuid, util::other::Uuid::Hash>> m_service_uuids; // 服务名，到节点uuid的映射
};

} // namespace cluster