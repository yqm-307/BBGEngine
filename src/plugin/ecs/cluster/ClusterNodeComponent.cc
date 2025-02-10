#include "ClusterNodeComponent.hpp"

namespace plugin::ecs::cluster
{

util::errcode::ErrOpt ClusterNodeComponent::AppendNode(std::shared_ptr<ClusterNode> node)
{
    auto uuid = node->GetUUID();
    if (uuid->IsNil())
        return util::errcode::ErrCode{"append node but a uuid is nil!", util::errcode::ErrType::CommonErr};

    return std::nullopt;
}

std::shared_ptr<ClusterNode> ClusterNodeComponent::RemoveNode(std::shared_ptr<ClusterNode> node)
{
    auto uuid = node->GetUUID();
    auto it = m_node_map.find(*uuid);
    if (it != m_node_map.end())
    {
        auto removed_node = it->second;
        m_node_map.erase(it);
        return removed_node;
    }

    return nullptr;
}

std::shared_ptr<ClusterNode> ClusterNodeComponent::FindNode(const util::other::Uuid& uuid) const
{
    auto it = m_node_map.find(uuid);
    if (it != m_node_map.end())
        return it->second;
    
    return nullptr;
}

void ClusterNodeComponent::ActiveNode(const util::other::Uuid& uuid)
{
    auto it = m_node_map.find(uuid);
    if (it != m_node_map.end())
    {
        it->second->Active();
    }
}

}