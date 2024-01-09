#pragma once
#include "Node.hpp"


template<typename TShareData>
Node<TShareData>::~Node() {};

template<typename TShareData>
void Node<TShareData>::OnProcess(TShareData& share_data) { 
    if (!m_has_executed && m_init_callback)
        m_init_callback(share_data); 
    m_has_executed = true;
}


//////////////////////////////////////////////
//////// 次序节点

template<typename TShareData>
SequencesNode<TShareData>::SequencesNode()
    :Node<TShareData>(emNodeType::Sequences)
{
}

template<typename TShareData>
SequencesNode<TShareData>::~SequencesNode()
{
}

template<typename TShareData>
emNodeStatus SequencesNode<TShareData>::Process(TShareData& share_data)
{
    /* 次序节点，所有子节点都执行 Success 才算成功，否则算做 Failed */

    for (auto&& it_node : BaseType::m_sub_nodes)
    {
        auto node = it_node.second;
        /* 已经执行完的节点不会再次执行，且返回上次执行的结果 */
        if (node->Status() != Running && node->Status() != emNodeStatus::UnInited) {
            return BaseType::m_status;
        }

        /* 如果是未执行过的则执行一次初始化（用户提供） */
        if (node->Status() == UnInited) {
            BaseType::OnProcess(share_data);
        }

        auto result = node->Process(share_data);

        if (result == emNodeStatus::Success) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else if (result == emNodeStatus::Running) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else {
            BaseType::m_status = emNodeStatus::Failed;
            break;
        }
    }

    return BaseType::m_status;
}

template<typename TShareData>
bool SequencesNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}



//////////////////////////////////////////////
//////// 选择节点

template<typename TShareData>
SelectorNode<TShareData>::SelectorNode()
    :Node<TShareData>(emNodeType::Selector)
{
}

template<typename TShareData>
SelectorNode<TShareData>::~SelectorNode()
{
}

template<typename TShareData>
emNodeStatus SelectorNode<TShareData>::Process(TShareData& share_data)
{
    /* 选择节点，所有子节点只要有一个执行 Success 就算成功，否则算做 Failed */

    for (auto&& it_node : BaseType::m_sub_nodes)
    {
        auto node = it_node->second;
        /* 已经执行完的节点不会再次执行，且返回上次执行的结果 */
        if (node->Status() != Running && node->Status() != emNodeStatus::UnInited) {
            return BaseType::m_status;
        }

        /* 如果是未执行过的则执行一次初始化（用户提供） */
        if (node->Status() == UnInited) {
            BaseType::OnProcess(share_data);
        }

        auto result = node.Process(share_data);

        if (result == emNodeStatus::Success) {
            BaseType::m_status = emNodeStatus::Success;
            break;
        } else if (result == emNodeStatus::Running) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        }
    }

    return BaseType::m_status;
}

template<typename TShareData>
bool SelectorNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}

//////////////////////////////////////////////
//////// 重复节点

template<typename TShareData>
RepeaterNode<TShareData>::RepeaterNode()
    :Node<TShareData>(emNodeType::Repeater)
{
}

template<typename TShareData>
RepeaterNode<TShareData>::~RepeaterNode()
{
}

template<typename TShareData>
emNodeStatus RepeaterNode<TShareData>::Process(TShareData& share_data)
{
    /* 重复节点，返回结果，并继续执行 */

    for (auto&& it_node : BaseType::m_sub_nodes)
    {
        auto node = it_node->second;
        /* 已经执行完的节点不会再次执行，且返回上次执行的结果 */
        if (node->Status() != Running && node->Status() != emNodeStatus::UnInited) {
            return BaseType::m_status;
        }

        /* 如果是未执行过的则执行一次初始化（用户提供） */
        if (node->Status() == UnInited) {
            BaseType::OnProcess(share_data);
        }

        auto result = node.Process(share_data);

        if (result == emNodeStatus::Success) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else if (result == emNodeStatus::Running) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else {
            BaseType::m_status = emNodeStatus::Failed;
            break;
        }
    }

    return BaseType::m_status;
}

template<typename TShareData>
bool RepeaterNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}


//////////////////////////////////////////////
//////// 重复直至失败节点

template<typename TShareData>
RepeatUntilFailNode<TShareData>::RepeatUntilFailNode()
    :Node<TShareData>(emNodeType::RepeatUntilFail)
{
}

template<typename TShareData>
RepeatUntilFailNode<TShareData>::~RepeatUntilFailNode()
{
}

template<typename TShareData>
emNodeStatus RepeatUntilFailNode<TShareData>::Process(TShareData& share_data)
{
    /* 重复直至失败节点，返回结果，并继续执行 */

    if (BaseType::m_status == emNodeStatus::Failed)
        return BaseType::m_status;

    bool has_failed = false;

    while (!has_failed) 
    for (auto&& it_node : BaseType::m_sub_nodes)
    {
        auto node = it_node.second;

        /* 如果是未执行过的则执行一次初始化（用户提供） */
        if (node->Status() == UnInited) {
            BaseType::OnProcess(share_data);
        }

        auto result = node->Process(share_data);

        if (result == emNodeStatus::Success) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else if (result == emNodeStatus::Running) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else {
            BaseType::m_status = emNodeStatus::Failed;
            has_failed = true;
            break;
        }
    }

    return BaseType::m_status;
}

template<typename TShareData>
bool RepeatUntilFailNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}

//////////////////////////////////////////////
//////// 逆变节点

template<typename TShareData>
InverterNode<TShareData>::InverterNode()
    :Node<TShareData>(emNodeType::Inverter)
{
}

template<typename TShareData>
InverterNode<TShareData>::~InverterNode()
{
}

template<typename TShareData>
emNodeStatus InverterNode<TShareData>::Process(TShareData& share_data)
{
    /* 反转节点的执行结果 */
    if (BaseType::m_status != emNodeStatus::Running || BaseType::m_status != UnInited)
        return !BaseType::m_status;

    for (auto&& it_node : BaseType::m_sub_nodes)
    {
        auto node = it_node->second;

        /* 如果是未执行过的则执行一次初始化（用户提供） */
        if (node->Status() == UnInited) {
            BaseType::OnProcess(share_data);
        }

        auto result = node.Process(share_data);

        if (result == emNodeStatus::Success) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else if (result == emNodeStatus::Running) {
            BaseType::m_status = emNodeStatus::Running;
            continue;
        } else {
            BaseType::m_status = emNodeStatus::Failed;
            break;
        }
    }

    return BaseType::m_status;
}

template<typename TShareData>
bool InverterNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}

//////////////////////////////////////////////
//////// 行为节点

template<typename TShareData>
ActionNode<TShareData>::ActionNode()
    :Node<TShareData>(emNodeType::Leaf)
{
}

template<typename TShareData>
ActionNode<TShareData>::~ActionNode()
{
}

template<typename TShareData>
emNodeStatus ActionNode<TShareData>::Process(TShareData& share_data)
{
    return m_active_callback(share_data);
}

template<typename TShareData>
bool ActionNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}

template<typename TShareData>
void ActionNode<TShareData>::SetActive(const OnActiveCallback& active_event)
{
    m_active_callback = active_event;
}


//////////////////////////////////////////////
//////// 条件节点

template<typename TShareData>
ConditionalNode<TShareData>::ConditionalNode()
    :Node<TShareData>(emNodeType::Conditional)
{
}

template<typename TShareData>
ConditionalNode<TShareData>::~ConditionalNode()
{
}

template<typename TShareData>
emNodeStatus ConditionalNode<TShareData>::Process(TShareData& share_data)
{
    return m_cond_expr(share_data);
}

template<typename TShareData>
bool ConditionalNode<TShareData>::__CheckCanSetNodes(std::map<int, typename BaseType::NodeBase*> nodes)
{
    return nodes.size() > 0;
}

template<typename TShareData>
void ConditionalNode<TShareData>::SetCondExpr(const CondCallback& cond_expr)
{
    m_cond_expr = cond_expr;
}