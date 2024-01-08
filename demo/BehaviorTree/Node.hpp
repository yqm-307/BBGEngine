#pragma once
#include <vector>
#include <map>
#include <functional>

enum emNodeType
{
    Sequences   = 1, // 次序节点
    Selector    = 2, // 选择节点
    Inverter    = 3, // 逆变节点
    Successder  = 4, // 成功节点
    Repeater    = 5, // 重复节点 repeat until fail

};

enum emProcessResult
{
    Failed      = 1,
    Success     = 2,
    Running     = 3,
};

template<typename TShareData>
class Node
{
public:
    typedef std::function<emProcessResult(TShareData&)> OnActiveCallback;
    typedef std::function<void()> OnInitCallback;

    Node(emNodeType type, Node* parent):m_type(type),m_parent(parent) {}
    ~Node() {}
    
    emNodeType Type() { return m_type; }
    Node* Parent() { return m_parent; }

    void Init(std::map<int, Node*> nodes) { m_sub_nodes = nodes; }

    emProcessResult Process(TShareData& share_data);
private:
    emNodeType              m_type;
    Node*                   m_parent;
    std::map<int, Node*>    m_sub_nodes;
    emProcessResult         m_cur_result{true};

    OnActiveCallback        m_active_callback;
    OnInitCallback          m_init_callback;
};


template<typename TShareData>
emProcessResult Node<TShareData>::Process(TShareData& share_data)
{
    for (auto&& child_node : m_sub_nodes) {
        switch (m_type) {
            case emNodeType::Inverter:
                m_cur_result |= ;
                break;
            case emNodeType::Repeater:
                break;
            case emNodeType::Selector:
                break;
            case emNodeType::Sequences:
                break;
            case emNodeType::Successder:
                break;
            default:
                break;
        }
    }
}