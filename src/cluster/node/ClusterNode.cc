#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNode.hpp>
#include <cluster/protocol/N2RProtocol.hpp>

namespace cluster
{

RpcSerializer serializer;

ClusterNode::ClusterNode():
    m_rpc_clients(10)
{
}

ClusterNode::~ClusterNode()
{
}

void ClusterNode::Init(const bbt::net::IPAddress& listen_addr, const bbt::net::IPAddress& registery_addr, int timeout)
{
    m_listen_addr = listen_addr;
    m_registery_addr = registery_addr;
    m_connect_timeout = timeout;
    
    m_network = std::make_shared<bbt::network::libevent::Network>();
    m_registery_client = std::make_shared<RegisteryClient>(std::static_pointer_cast<bbt::network::base::NetworkBase>(m_network), m_registery_addr);
    m_rpc_server = std::make_shared<RpcServer>(shared_from_this(), m_listen_addr, m_connect_timeout);
    
}

util::other::Uuid::SPtr ClusterNode::GetUUID() const
{
    return m_uuid;
}

NodeState ClusterNode::GetNodeState()
{
    return m_state;
}

const std::string& ClusterNode::GetName() const
{
    return m_service_name;
}

void ClusterNode::Start()
{
}


void ClusterNode::Active()
{
    auto buffer = serializer.Serialize(
        N2R_KEEPALIVE_REQ,
        m_uuid->ToString(),
        bbt::clock::gettime() / 1000);

    // 发送心跳包
    SendToRegistery(buffer);
}

void ClusterNode::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void ClusterNode::Online()
{
    m_state = NODESTATE_ONLINE;
}

void ClusterNode::Clear()
{
    m_last_active_time = -1;
    m_service_name = "";
    m_state = NODESTATE_DEFAULT;
    m_uuid = nullptr;
}

util::errcode::ErrOpt ClusterNode::RemoteCall(const std::string& method, bbt::core::Buffer& buffer)
{
    // 找到本地方法

    // 解析协议头

    // 执行方法
}

util::errcode::ErrOpt OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply)
{

}

}