#pragma once
#include <util/other/Uuid.hpp>
#include <util/network/Connection.hpp>
#include <bbt/base/net/IPAddress.hpp>
#include <cluster/ClusterDefine.hpp>
#include <cluster/rpc/RpcClient.hpp>
#include <cluster/rpc/RpcServer.hpp>
#include <cluster/registery/RegisteryClient.hpp>

namespace cluster
{

/**
 * @brief 集群节点基类
 * 负责维护节点状态，节点间通信，节点注册中心通信
 * 
 * 基类负责实现节点间通信行为，注册中心通信行为；派生类负责实现网络通信
 * 
 */
class ClusterNodeBase:
    public bbt::templateutil::noncopyable,
    public std::enable_shared_from_this<ClusterNodeBase>
{
public:
    ClusterNodeBase();
    virtual ~ClusterNodeBase();

    void                                Init();
    void                                SetRegisteryAddr(const bbt::net::IPAddress& addr);
    void                                Start();
    virtual void                        Active();
    virtual void                        Offline();
    virtual void                        Online();
    virtual void                        Clear();

    virtual util::other::Uuid::SPtr     GetUUID() const;
    virtual NodeState                   GetNodeState();
    virtual const std::string&          GetName() const;


    virtual util::errcode::ErrOpt       RemoteCall(const std::string& method, bbt::core::Buffer& buffer);
    virtual void                        OnRemoteCall(bbt::core::Buffer& reply);

    // 基类定义节点间网络传输行为
    virtual util::errcode::ErrOpt       SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer) = 0;
    virtual util::errcode::ErrOpt       RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer) = 0;
    // 基类定义注册中心网络传输行为
    virtual void                        SendToRegistery(bbt::core::Buffer& buffer) = 0;
    virtual void                        RecvFromRegistery(bbt::core::Buffer& buffer) = 0;
    virtual void                        OnError(const bbt::errcode::Errcode& err) = 0;

private:
    NodeState               m_state{NODESTATE_DEFAULT};
    std::string             m_service_name{""};
    util::other::Uuid::SPtr m_uuid;
    time_t                  m_last_active_time{-1};    
};

} // namespace cluster