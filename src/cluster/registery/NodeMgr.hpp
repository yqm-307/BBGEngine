#pragma once
#include <util/other/Uuid.hpp>
#include <cluster/registery/RegisterInfo.hpp>

namespace cluster
{

/**
 * 节点信息管理器
 */
class NodeMgr
{
public:
    NodeMgr() = default;
    ~NodeMgr() = default;

    void                    Update();

    void                    RegisterNode(util::other::Uuid uuid, NodeRegInfo::SPtr info);
    void                    NodeOffline(util::other::Uuid uuid);
    void                    NodeOffline(bbt::network::ConnId connid);
    NodeRegInfo::SPtr       GetNodeInfo(util::other::Uuid uuid);
    NodeRegInfo::SPtr       GetNodeInfo(bbt::network::ConnId connid);
    NodeState               GetNodeState(util::other::Uuid uuid) const;
    std::vector<NodeRegInfo::SPtr> GetAllNodeInfo() const;
private:
    void                    DebugPrint();
private:
    std::unordered_map<util::other::Uuid, NodeRegInfo::SPtr, util::other::Uuid::Hash>
                            m_node_map; // 保存了全节点信息的map
    std::unordered_map<bbt::network::ConnId, util::other::Uuid>
                            m_connid_map; // 保存了connid和uuid的映射
};

} // namespace cluster