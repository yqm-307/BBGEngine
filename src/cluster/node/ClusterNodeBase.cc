#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNodeBase.hpp>

namespace cluster
{

ClusterNodeBase::ClusterNodeBase(const ClusterNodeBase& other):
    m_state(other.m_state),
    m_service_name(other.m_service_name),
    m_uuid(other.m_uuid),
    m_last_active_time(other.m_last_active_time)
{
}

ClusterNodeBase::ClusterNodeBase(ClusterNodeBase&& other):
    m_state(other.m_state),
    m_service_name(std::move(other.m_service_name)),
    m_uuid(std::move(other.m_uuid)),
    m_last_active_time(other.m_last_active_time)
{
    other.Clear();
}


ClusterNodeBase::ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name):
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>())
{
}

ClusterNodeBase::ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name, const util::other::Uuid& uuid):
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>(uuid))
{
}

ClusterNodeBase::ClusterNodeBase(const bbt::net::IPAddress& addr, const std::string& service_name, util::other::Uuid&& uuid):
    m_service_name(service_name),
    m_uuid(std::make_shared<util::other::Uuid>(std::move(uuid)))
{
}

ClusterNodeBase::~ClusterNodeBase()
{
}

util::other::Uuid::SPtr ClusterNodeBase::GetUUID() const
{
    return m_uuid;
}

NodeState ClusterNodeBase::GetNodeState()
{
    return m_state;
}

const std::string& ClusterNodeBase::GetName() const
{
    return m_service_name;
}

void ClusterNodeBase::Active()
{
    // 更新下节点最后活跃时间
    m_last_active_time = bbt::clock::gettime() / 1000;
}

void ClusterNodeBase::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void ClusterNodeBase::Online()
{
    m_state = NODESTATE_ONLINE;
}

ClusterNodeBase& ClusterNodeBase::operator=(ClusterNodeBase&& other)
{
    *this = std::move(other);
    return *this;
}

ClusterNodeBase& ClusterNodeBase::operator=(const ClusterNodeBase& other)
{
    *this = other;
    return *this;
}

void ClusterNodeBase::Clear()
{
    m_last_active_time = -1;
    m_service_name = "";
    m_state = NODESTATE_DEFAULT;
    m_uuid = nullptr;
}


}