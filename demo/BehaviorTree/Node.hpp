#pragma once
#include <vector>
#include <map>
#include <functional>

enum emNodeType
{
    Sequences       = 1, // 次序节点
    Selector        = 2, // 选择节点
    Inverter        = 3, // 逆变节点
    Successder      = 4, // 成功节点
    Repeater        = 5, // 重复节点 
    RepeatUntilFail = 6, // 重复直至失败
    Leaf            = 7, // 叶节点（事件节点）
    Conditional     = 8, // 条件节点

};

enum emNodeStatus
{
    /* 执行完毕的节点状态只有成功或者失败 */
    Failed      = 1,
    Success     = 2,
    /* 未执行完成的节点是 Running 或 UnInited */
    Running     = 3,
    UnInited    = 4,
};

template<typename TShareData>
class Node
{
public:
    typedef std::function<void(TShareData&)> OnInitCallback;
    typedef Node<TShareData> NodeBase;

    Node(emNodeType type):m_type(type) {}
    virtual ~Node() = 0;
    
    emNodeType Type() { return m_type; }
    emNodeStatus Status() { return m_status; }

    void Init(std::map<int, NodeBase*> nodes) { m_sub_nodes = nodes; }

    /**
     * @brief 执行以本节点为根的子行为树的行为，并返回执行结果
     * 
     * @param share_data 整个行为树的共享数据 //XXX 也许有更好的实现方式
     * @return emNodeStatus 
     */
    virtual emNodeStatus Process(TShareData& share_data) = 0;

protected:
    /* 检查nodes对于本节点是否为合法的子节点集合（比如逆变节点的子节点只能有一个之类的限制） */
    virtual bool __CheckCanSetNodes(std::map<int, NodeBase*> nodes) = 0;
    /* 可重入 */
    void OnProcess(TShareData& share_data);

    const emNodeType            m_type;
    std::map<int, NodeBase*>    m_sub_nodes;

    OnInitCallback          m_init_callback{nullptr};
    bool                    m_has_executed{false};
    emNodeStatus            m_status{emNodeStatus::UnInited};
};


//////////////////////////////////////////////
//////// 次序节点
template<typename TShareData>
class SequencesNode:
    public Node<TShareData>
{
public:
    typedef Node<TShareData> BaseType;
    SequencesNode();
    virtual ~SequencesNode();
    virtual emNodeStatus Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;

};

//////////////////////////////////////////////
//////// 选择节点
template<typename TShareData>
class SelectorNode:
    public Node<TShareData>
{
public:
    typedef Node<TShareData> BaseType;
    SelectorNode();
    virtual ~SelectorNode();
    virtual emNodeStatus Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;
};

//////////////////////////////////////////////
//////// 重复节点
template<typename TShareData>
class RepeaterNode:
    public Node<TShareData>
{
public:
    typedef Node<TShareData> BaseType;
    RepeaterNode();
    virtual ~RepeaterNode();
    virtual emNodeStatus Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;
};

//////////////////////////////////////////////
//////// 重复直到失败节点
template<typename TShareData>
class RepeatUntilFailNode:
    public Node<TShareData>
{
public:
    typedef Node<TShareData> BaseType;
    RepeatUntilFailNode();
    virtual ~RepeatUntilFailNode();
    virtual emNodeStatus Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;
};

//////////////////////////////////////////////
//////// 逆变节点
template<typename TShareData>
class InverterNode:
    public Node<TShareData>
{
public:
    typedef Node<TShareData> BaseType;
    InverterNode();
    virtual ~InverterNode();
    virtual emNodeStatus Process(TShareData& share_data) override;
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;
};

//////////////////////////////////////////////
//////// 行为节点
template<typename TShareData>
class ActionNode:
    public Node<TShareData>
{
public:
    typedef std::function<emNodeStatus(TShareData&)> OnActiveCallback;
    typedef Node<TShareData> BaseType;

    ActionNode();
    virtual ~ActionNode();

    virtual emNodeStatus Process(TShareData& share_data) override;
    void SetActive(const OnActiveCallback& active_event);
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;
    OnActiveCallback        m_active_callback;
};

//////////////////////////////////////////////
//////// 条件节点
template<typename TShareData>
class ConditionalNode:
    public Node<TShareData>
{
public:
    typedef std::function<emNodeStatus(TShareData&)> CondCallback;
    typedef Node<TShareData> BaseType;

    ConditionalNode();
    virtual ~ConditionalNode();

    virtual emNodeStatus Process(TShareData& share_data) override;
    void SetCondExpr(const CondCallback& cond_expr);
protected:
    virtual bool __CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes) override;

    CondCallback    m_cond_expr;
};


#include "__TNode.hpp"