#pragma once
#include <cluster/ClusterDefine.hpp>
#include <util/errcode/ErrCodeDef.hpp>

namespace cluster
{

/**
 * @brief 节点注册信息
 */
class NodeRegInfo
{
public:
    typedef std::shared_ptr<NodeRegInfo> SPtr;
    NodeRegInfo();
    ~NodeRegInfo();

    NodeRegInfo& operator=(const NodeRegInfo&);

    void                Clear();
    void                Init(const util::other::Uuid& uuid, const bbt::network::IPAddress& addr);

    void                Update();
    void                OnHeartBeat();
    util::errcode::ErrOpt AddMethod(const std::string& method_name);
    void                DelMethod(const std::string& method_name);
    bool                HasMethod(const std::string& method_name) const;
    std::vector<std::string> GetMethods() const;
    void                SetStatus(NodeState state);
    NodeState           GetStatus() const;
    const util::other::Uuid& GetUuid() const;
    bbt::network::ConnId GetConnId() const;
    void                SetConnId(bbt::network::ConnId connid);
    const bbt::network::IPAddress& GetNodeAddr() const;
    
private:
    NodeState                       m_state{NodeState::NODESTATE_UNREGISTER}; // 节点状态
    util::other::Uuid               m_uuid; // 节点id
    bbt::network::IPAddress        m_node_addr; // 节点地址
    bbt::network::ConnId            m_connid{0};
    std::unordered_set<std::string> m_method_info_map;  // 方法名
    bbt::core::clock::Timestamp<>   m_last_heartbeat{bbt::core::clock::now()}; // 最后一次心跳时间
    const int                       m_heartbeat_timeout_ms{BBGENGINE_HEARTBEAT_TIMEOUT_MS}; // 心跳超时时间

};

} // namespace cluster