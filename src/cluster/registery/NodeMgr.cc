#include <cluster/registery/NodeMgr.hpp>

namespace cluster
{

void NodeMgr::RegisterNode(util::other::Uuid uuid, NodeRegInfo::SPtr info)
{
    m_node_map[uuid] = info;
    m_connid_map[info->GetConnId()] = uuid;
}

void NodeMgr::NodeOffline(util::other::Uuid uuid)
{
    if (m_node_map.find(uuid) != m_node_map.end()) {
        auto& node = m_node_map[uuid];
        m_connid_map.erase(node->GetConnId());
        node->SetStatus(NodeState::NODESTATE_OFFLINE);
        node->SetConnId(0);
    }
}

void NodeMgr::NodeOffline(bbt::network::ConnId connid)
{
    if (connid == 0)
        return;

    auto it = m_connid_map.find(connid);
    if (it != m_connid_map.end()) {
        NodeOffline(it->second);
        m_connid_map.erase(it);
    }

}

NodeRegInfo::SPtr NodeMgr::GetNodeInfo(util::other::Uuid uuid)
{
    auto it = m_node_map.find(uuid);
    if (it == m_node_map.end())
        return nullptr;
    return it->second;
}

NodeRegInfo::SPtr NodeMgr::GetNodeInfo(bbt::network::ConnId connid)
{
    auto it = m_connid_map.find(connid);
    if (it == m_connid_map.end())
        return nullptr;
    return GetNodeInfo(it->second);
}


NodeState NodeMgr::GetNodeState(util::other::Uuid uuid) const
{
    auto it = m_node_map.find(uuid);
    if (it == m_node_map.end())
        return NodeState::NODESTATE_UNREGISTER;
    return it->second->GetStatus();
}

std::vector<NodeRegInfo::SPtr> NodeMgr::GetAllNodeInfo() const
{
    std::vector<NodeRegInfo::SPtr> ret;
    for (auto& it : m_node_map) {
        ret.push_back(it.second);
    }
    return ret;
}


void NodeMgr::DebugPrint()
{
    printf("=====================>> nodemgr debug info <<=====================\n");
    for (auto& it : m_node_map) {
        auto& node = it.second;

        printf("[NodeMgr::DebugInfo] uuid: %s, connid: %d, status: %d\n",
                 node->GetUuid().ToString().c_str(), node->GetConnId(), node->GetStatus());
        if (node->GetStatus() == NodeState::NODESTATE_OFFLINE)
            Assert(m_connid_map.find(node->GetConnId()) == m_connid_map.end());
        if (node->GetStatus() == NodeState::NODESTATE_ONLINE)
            Assert(m_connid_map.find(node->GetConnId()) != m_connid_map.end());
    }
}

void NodeMgr::Update()
{
    std::vector<util::other::Uuid> offline_nodes;
    for (auto&& it : m_node_map) {
        auto& node = it.second;

        node->Update();
        if (node->GetStatus() == NodeState::NODESTATE_OFFLINE) {
            offline_nodes.push_back(it.first);
        }
    }

    for (auto&& uuid : offline_nodes) {
        auto it = m_node_map.find(uuid);
        if (it != m_node_map.end()) {
            auto connid = it->second->GetConnId();
            m_node_map.erase(uuid);
            m_connid_map.erase(connid);
        }
    }

    DebugPrint();
}



} // namespace cluster