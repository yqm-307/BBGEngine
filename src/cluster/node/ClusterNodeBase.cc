#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNodeBase.hpp>
#include <cluster/protocol/N2RProtocol.hpp>

namespace cluster
{

RpcSerializer serializer;

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

void ClusterNodeBase::SetRegisteryAddr(const bbt::net::IPAddress& addr)
{
    m_registery_addr = addr;
}


void ClusterNodeBase::Start()
{
    _AsyncConnectToNode(bbt::net::IPAddress(), [&](std::shared_ptr<R2NConnection> conn)
    {
        if (conn == nullptr)
            return;

        m_registery_conn = conn;
        // 注册到注册中心
        bbt::core::Buffer buffer;
        N2R_Handshake_Req req;
        req.head.protocol_type = N2R_HANDSHAKE_REQ;
        req.head.protocol_length = sizeof(req);
        m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid));
        req.head.timestamp_ms = bbt::clock::gettime();

        buffer.WriteString((char*)&req, sizeof(req));
        SendToRegistery(buffer);
    });
}


void ClusterNodeBase::Active()
{
    auto buffer = serializer.Serialize(
        N2R_KEEPALIVE_REQ,
        m_uuid->ToString(),
        bbt::clock::gettime() / 1000);

    // 发送心跳包
    SendToRegistery(buffer);
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