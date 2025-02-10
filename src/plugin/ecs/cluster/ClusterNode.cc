#include <bbt/base/clock/Clock.hpp>
#include "ClusterNode.hpp"

namespace plugin::ecs::cluster
{

ClusterNode::ClusterNode(const ClusterNode& other):
    m_address(other.m_address),
    m_state(other.m_state),
    m_service_name(other.m_service_name),
    m_uuid(other.m_uuid),
    m_last_active_time(other.m_last_active_time)
{
}

ClusterNode::ClusterNode(ClusterNode&& other):
    m_address(std::move(other.m_address)),
    m_state(other.m_state),
    m_service_name(std::move(other.m_service_name)),
    m_uuid(std::move(other.m_uuid)),
    m_last_active_time(other.m_last_active_time)
{
    other.Clear();
}


ClusterNode::ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name):
    m_address(addr),
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>())
{
}

ClusterNode::ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name, const util::other::Uuid& uuid):
    m_address(addr),
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>(uuid))
{
}

ClusterNode::ClusterNode(const bbt::net::IPAddress& addr, const std::string& service_name, util::other::Uuid&& uuid):
    m_address(addr),
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>(std::move(uuid)))
{
}

ClusterNode::~ClusterNode()
{
}

util::other::Uuid::SPtr ClusterNode::GetUUID() const
{
    return m_uuid;
}

NodeState ClusterNode::GetNodeState()
{
    return m_state;
}

const bbt::net::IPAddress& ClusterNode::GetAddr() const
{
    return m_address;
}

const std::string& ClusterNode::GetName() const
{
    return m_service_name;
}

void ClusterNode::Active()
{
    // 更新下节点最后活跃时间
    m_last_active_time = bbt::clock::gettime() / 1000;
}

void ClusterNode::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void ClusterNode::Online()
{
    m_state = NODESTATE_ONLINE;
}

ClusterNode& ClusterNode::operator=(ClusterNode&& other)
{
    *this = std::move(other);
    return *this;
}

ClusterNode& ClusterNode::operator=(const ClusterNode& other)
{
    *this = other;
    return *this;
}

void ClusterNode::Clear()
{
    m_address.Clear();
    m_last_active_time = -1;
    m_service_name = "";
    m_state = NODESTATE_DEFAULT;
    m_uuid = nullptr;
}


}