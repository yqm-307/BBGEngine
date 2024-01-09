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

template<typename TShareData, emNodeType NodeType>
class Node
{
public:
    typedef std::function<emProcessResult(TShareData&)> OnActiveCallback;
    typedef std::function<void()> OnInitCallback;
    typedef Node<TShareData, NodeType> NodeBase;

    Node(emNodeType type, NodeBase* parent):m_type(type),m_parent(parent) {}
    virtual ~Node() = 0
    
    emNodeType Type() { return NodeType; }
    NodeBase* Parent() { return m_parent; }

    bool Init(std::map<int, NodeBase*> nodes) { m_sub_nodes = nodes; }

    /**
     * @brief 执行以本节点为根的子行为树的行为，并返回执行结果
     * 
     * @param share_data 整个行为树的共享数据 //XXX 不算优雅的实现方式
     * @return emProcessResult 
     */
    virtual emProcessResult Process(TShareData& share_data) = 0;

protected:
    /* 检查nodes对于本节点是否为合法的子节点集合（比如逆变节点的子节点只能有一个之类的限制） */
    virtual bool __CheckCanSetNodes(std::map<int, typename Node<TShareData, emNodeType::Sequences>::NodeBase*> nodes) = 0;

    NodeBase*                   m_parent;
    std::map<int, NodeBase*>    m_sub_nodes;

    OnActiveCallback        m_active_callback;
    OnInitCallback          m_init_callback;
};


template<typename TShareData>
class SequencesNode:
    public Node<TShareData, emNodeType::Sequences>
{
public:
    virtual emProcessResult Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, NodeBase*> nodes) override;

};

template<typename TShareData>
class SelectorNode:
    public Node<TShareData, emNodeType::Selector>
{
public:
    virtual emProcessResult Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, Node*> nodes) override;
};


#include "__TNode.hpp"