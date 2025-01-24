#pragma once
#include <util/other/Uuid.hpp>
#include <bbt/base/net/IPAddress.hpp>
#include "share/ecs/cluster/ClusterDefine.hpp"

namespace share::ecs::cluster
{

class ClusterNode
{
public:

    explicit ClusterNode(const ClusterNode& other);
    explicit ClusterNode(ClusterNode&& other);
    ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name);
    ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name, const util::other::Uuid& uuid);
    ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name, util::other::Uuid&& uuid);
    virtual ~ClusterNode();

    ClusterNode& operator=(ClusterNode&& other);
    ClusterNode& operator=(const ClusterNode& other);

    virtual void Active();

    virtual void Offline();
    virtual void Online();
    virtual void Clear();

    virtual util::other::Uuid::SPtr     GetUUID() const;
    virtual NodeState                   GetNodeState();
    virtual const bbt::net::IPAddress&  GetAddr() const;
    virtual const std::string&          GetName() const;
    
private:
    bbt::net::IPAddress     m_address;
    NodeState               m_state{NODESTATE_DEFAULT};
    std::string             m_service_name{""};
    util::other::Uuid::SPtr m_uuid;
    time_t                  m_last_active_time{-1};
};

}