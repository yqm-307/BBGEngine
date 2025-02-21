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

class ClusterNodeBase
{
public:
    explicit ClusterNodeBase(const ClusterNodeBase& other);
    explicit ClusterNodeBase(ClusterNodeBase&& other);
    ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name);
    ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name, const util::other::Uuid& uuid);
    ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name, util::other::Uuid&& uuid);
    virtual ~ClusterNodeBase();

    ClusterNodeBase& operator=(ClusterNodeBase&& other);
    ClusterNodeBase& operator=(const ClusterNodeBase& other);

    virtual void Active();
    virtual void Offline();
    virtual void Online();
    virtual void Clear();

    virtual util::other::Uuid::SPtr     GetUUID() const;
    virtual NodeState                   GetNodeState();
    virtual const std::string&          GetName() const;

    // 基类定义网络传输行为
    virtual util::errcode::ErrOpt       RemoteCall(const std::string& method, bbt::core::Buffer& buffer) = 0;
    virtual util::errcode::ErrOpt       AysncRemoteCall(
                                            const std::string& method,
                                            bbt::core::Buffer& request,
                                            std::function<void()> onreply_cb) = 0;
    virtual void                        OnRemoteCall(bbt::core::Buffer& reply) = 0;

    // 基类定义网络传输行为
    virtual void                        SendToRegistery(bbt::core::Buffer& buffer) = 0;
    virtual void                        RecvFromRegistery(bbt::core::Buffer& buffer) = 0;

protected:
    virtual void                        _AsyncConnectToNode(const bbt::net::IPAddress& addr, std::function<void()> onconn_cb) = 0;

private:
    NodeState               m_state{NODESTATE_DEFAULT};
    std::string             m_service_name{""};
    util::other::Uuid::SPtr m_uuid;
    time_t                  m_last_active_time{-1};
    std::unordered_map<std::string, RpcCallback>      m_method_map;
    std::unordered_map<std::string, std::vector<std::shared_ptr<RpcClient>>> m_service_map;
    // std::shared_ptr<RpcServer>      m_rpc_server{nullptr};
    // std::shared_ptr<NRConnection>   m_registery_conn{nullptr};    // 注册中心连接
};

}