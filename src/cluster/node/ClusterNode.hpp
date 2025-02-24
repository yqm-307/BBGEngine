#pragma once
#include <util/other/Uuid.hpp>
#include <util/network/Connection.hpp>
#include <bbt/base/net/IPAddress.hpp>
#include <cluster/ClusterDefine.hpp>
#include <cluster/rpc/RpcClient.hpp>
#include <cluster/rpc/RpcServer.hpp>
#include <cluster/registery/RegisteryClient.hpp>
#include <cluster/protocol/N2NProtocol.hpp>
#include <cluster/protocol/N2RProtocol.hpp>

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
    public bbt::templateutil::noncopyable,
    public std::enable_shared_from_this<ClusterNode>
{
public:
    ClusterNode();
    virtual ~ClusterNode();

    void                                Init(const bbt::net::IPAddress& listen_addr, const bbt::net::IPAddress& registery_addr, int timeout);
    void                                Start();
    virtual void                        Active();
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
    // 基类定义注册中心网络传输行为
    virtual void                        SendToRegistery(bbt::core::Buffer& buffer);
    virtual void                        RecvFromRegistery(bbt::core::Buffer& buffer);
    virtual void                        OnError(const bbt::errcode::Errcode& err) = 0;
protected:
    std::shared_ptr<util::other::Uuid>  GetRandomUuidByMethod(const std::string& method);

    // n2n
    util::errcode::ErrOpt               N2NDispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt               DoHeartBeat(bbt::network::ConnId id, N2N_KeepAlive_Req* req);

    // n2r
    util::errcode::ErrOpt               N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len);
private:
    NodeState               m_state{NODESTATE_DEFAULT};
    std::string             m_service_name{""};
    util::other::Uuid::SPtr m_uuid;
    time_t                  m_last_active_time{-1};
    bbt::net::IPAddress     m_listen_addr;
    bbt::net::IPAddress     m_registery_addr;
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};

    std::shared_ptr<RpcServer>          m_rpc_server{nullptr};  // rpc节点间连接
    std::shared_ptr<RegisteryClient>    m_registery_client{nullptr};    // 与注册中心的连接
    std::unordered_map<util::other::Uuid, std::shared_ptr<RpcClient>> m_rpc_clients;  // 与其他节点的连接
    int m_connect_timeout{0};

    // 本地缓存
    std::unordered_map<std::string, std::unordered_set<util::other::Uuid>> m_service_uuids; // 服务名，到节点uuid的映射
};

} // namespace cluster