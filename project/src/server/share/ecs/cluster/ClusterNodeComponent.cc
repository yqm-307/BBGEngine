#include "ClusterNodeComponent.hpp"

namespace share::ecs::cluster
{

util::errcode::ErrOpt ClusterNodeComponent::AppendNode(std::shared_ptr<ClusterNode> node)
{
    auto uuid = node->GetUUID();
    if (uuid->IsNil())
        return util::errcode::ErrCode{"append node but a uuid is nil!", util::errcode::ErrType::CommonErr};


}

std::shared_ptr<ClusterNode> ClusterNodeComponent::RemoveNode(std::shared_ptr<ClusterNode> node)
{

}

std::shared_ptr<ClusterNode> ClusterNodeComponent::FindNode(const std::string& service_name) const
{

}

void ClusterNodeComponent::ActiveNode(const std::string& service_name)
{

}

}