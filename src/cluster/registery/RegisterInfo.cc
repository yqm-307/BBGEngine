#include <cluster/registery/RegisterInfo.hpp>
#include <util/errcode/ErrCodeDef.hpp>

namespace cluster
{

NodeRegInfo::NodeRegInfo()
{
}

NodeRegInfo::~NodeRegInfo()
{
}

NodeRegInfo& NodeRegInfo::operator=(const NodeRegInfo& other)
{
    m_state = other.m_state;
    m_uuid = other.m_uuid;
    m_node_addr = other.m_node_addr;
    m_connid = other.m_connid;
    m_method_info_map = other.m_method_info_map;
    m_last_heartbeat = other.m_last_heartbeat;
    return *this;
}

void NodeRegInfo::Clear()
{
    m_state = NodeState::NODESTATE_UNREGISTER;
    m_method_info_map.clear();
    m_uuid.Clear();
}

void NodeRegInfo::Init(const util::other::Uuid& uuid, const bbt::network::IPAddress& addr)
{
    m_uuid = uuid;
    m_node_addr = addr;
    m_state = NodeState::NODESTATE_ONLINE;
}

void NodeRegInfo::Update()
{
    auto time_diff = (bbt::core::clock::now() - m_last_heartbeat).count();
    if (time_diff > m_heartbeat_timeout_ms)
    {
        m_state = NODESTATE_OFFLINE;
    }
}

void NodeRegInfo::OnHeartBeat()
{
    m_last_heartbeat = bbt::core::clock::now();
}

util::errcode::ErrOpt NodeRegInfo::AddMethod(const std::string& method_name)
{
    if (m_method_info_map.find(method_name) != m_method_info_map.end())
        return util::errcode::Errcode("method already exist!", util::errcode::emErr::CommonErr);
    
    m_method_info_map.insert(method_name);
    return std::nullopt;
}

void NodeRegInfo::DelMethod(const std::string& method_name)
{
    m_method_info_map.erase(method_name);

    // 如果没有方法了，就下线
    if (m_method_info_map.empty())
    {
        m_state = NodeState::NODESTATE_OFFLINE;
    }
}

bool NodeRegInfo::HasMethod(const std::string& method_name) const
{
    return m_method_info_map.find(method_name) != m_method_info_map.end();
}

std::vector<std::string> NodeRegInfo::GetMethods() const
{
    std::vector<std::string> methods;
    for (auto&& method : m_method_info_map)
        methods.push_back(method);
    return methods;
}


NodeState NodeRegInfo::GetStatus() const
{
    return m_state;
}

void NodeRegInfo::SetStatus(NodeState state)
{
    m_state = state;
}

const util::other::Uuid& NodeRegInfo::GetUuid() const
{
    return m_uuid;
}

bbt::network::ConnId NodeRegInfo::GetConnId() const
{
    return m_connid;
}

void NodeRegInfo::SetConnId(bbt::network::ConnId connid)
{
    m_connid = connid;
}

const bbt::network::IPAddress& NodeRegInfo::GetNodeAddr() const
{
    return m_node_addr;
}

} // namespace cluster