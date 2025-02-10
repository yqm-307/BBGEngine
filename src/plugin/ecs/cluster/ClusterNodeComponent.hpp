#pragma once
#include <engine/ecs/component/Component.hpp>
#include <util/other/Uuid.hpp>
#include <util/errcode/ErrCode.hpp>
#include "ClusterNode.hpp"

namespace plugin::ecs::cluster
{

/**
 * @brief 集群节点组件
 * 
 * 组件赋予功能：
 * 可以与其他此组件对象促成网络
 * 且赋予以下功能：
 * 1、节点之间互连、通信
 * 2、节点之间自动保活
 * 
 */
class ClusterNodeComponent:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    virtual ~ClusterNodeComponent();

    // 已连接节点管理
    util::errcode::ErrOpt AppendNode(std::shared_ptr<ClusterNode> node);
    std::shared_ptr<ClusterNode> RemoveNode(std::shared_ptr<ClusterNode> node);
    std::shared_ptr<ClusterNode> FindNode(const util::other::Uuid& uuid) const;
    void ActiveNode(const util::other::Uuid& uuid);

    // 节点操作
private:
    explicit ClusterNodeComponent(const std::string& ip, short port);
    virtual void OnUpdate() override;

private:
    std::string         m_ip{""};
    short               m_port{-1};
    std::string         m_node_name{""};
    util::other::Uuid   m_node_uuid;
    NodeState           m_state{NodeState::NODESTATE_DEFAULT};

    /* 节点内维护其他节点数据 */
    std::unordered_map<util::other::Uuid, std::shared_ptr<ClusterNode>, util::other::Uuid::Hash>
                        m_node_map;

};

}