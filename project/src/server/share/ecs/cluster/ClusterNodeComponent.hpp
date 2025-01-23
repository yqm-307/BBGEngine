#pragma once
#include <engine/ecs/component/Component.hpp>
#include "share/ecs/cluster/ClusterDefine.hpp"

namespace share::ecs::cluster
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
    void AppendNode(const NodeInfo& node);    
    void RemoveNode(const NodeInfo& node);
    const NodeInfo& FindNode(const std::string& service_name);
    void ActiveNode(const std::string& service_name);

    // 节点操作
private:
    explicit ClusterNodeComponent(const std::string& ip, short port);
    virtual void OnUpdate() override;

private:
    std::string         m_ip;
    short               m_port;
    std::string         m_node_name;
    char                m_node_uuid[128];
    NodeState           m_state;
    std::unordered_map<std::string/*service_name*/, NodeInfo> 
                        m_node_map;

};

}