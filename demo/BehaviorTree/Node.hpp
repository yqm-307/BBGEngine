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
    Leaf        = 6, // 叶节点（事件节点）

};

enum emProcessResult
{
    Failed      = 1,
    Success     = 2,
    Running     = 3,
    UnInited    = 4,
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

    bool Init(std::map<int, Node*> nodes) { m_sub_nodes = nodes; }

    /**
     * @brief 执行以本节点为根的子行为树的行为，并返回执行结果
     * 
     * @param share_data 整个行为树的共享数据 //XXX 不算优雅的实现方式
     * @return emProcessResult 
     */
    emProcessResult Process(TShareData& share_data);

private:
    /* 检查nodes对于本节点是否为合法的子节点集合（比如逆变节点的子节点只能有一个之类的限制） */
    bool __CheckCanSetNodes(std::map<int, Node*> nodes);
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

    for (auto&& it : m_sub_nodes) {
        /* 第一次、Running状态下可以active */
        if (it->second.Type() == emProcessResult::Success || it->second.Type() == emProcessResult::Failed)
            continue;

        emProcessResult active_result = m_active_callback(share_data);

        /* 根据子行为树的执行结果，计算本节点的执行结果 */
        switch (m_type) {
            case emNodeType::Inverter:
                m_cur_result = !active_result;
                break;
            case emNodeType::Repeater:
                m_cur_result |= active_result;
                break;
            case emNodeType::Selector:
                m_cur_result |= active_result;
                break;
            case emNodeType::Sequences:
                m_cur_result &= active_result;
                break;
            case emNodeType::Successder:
                break;
            default:
                break;
        }
    }
}

#include "__TNode.hpp"