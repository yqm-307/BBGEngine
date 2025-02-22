#pragma once
#include <cluster/ClusterDefine.hpp>
#include <util/network/Connection.hpp>

namespace cluster
{

/**
 * @brief 节点注册信息
 */
class RegisterInfo
{
public:
    RegisterInfo();
    ~RegisterInfo();

    void Clear();
    void Init(util::other::Uuid uuid, bbt::network::ConnId conn_id);

    void AddMethod(const std::string& method_name);
    void DelMethod(const std::string& method_name);
    bool HasMethod(const std::string& method_name) const;
    void Active();
    bbt::network::ConnId GetConnId() const;
    
private:
    NodeState m_state{NodeState::NODESTATE_DEFAULT}; // 节点状态
    util::other::Uuid m_uuid; // 节点id
    std::unordered_set<std::string> m_method_info_map;  // 方法名
    bbt::clock::Timestamp<bbt::clock::ms> m_last_active_time; // 最后活跃时间
    bbt::network::ConnId m_conn_id{0}; // 连接id
    int m_heart_beat_interval;

};

} // namespace cluster