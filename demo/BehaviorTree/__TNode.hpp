#pragma once
#include "Node.hpp"


template<typename TShareData, emNodeType NodeType>
Node<TShareData, NodeType>::~Node() {};

// template<typename TShareData>
// bool Node<TShareData>::__CheckCanSetNodes(std::map<int, Node*> nodes)
// {
//     switch (m_type) {
//         /* 存在至少一个节点 */
//         case emNodeType::Sequences:
//         case emNodeType::Selector:
//         case emNodeType::Successder:
//         case emNodeType::Inverter:
//         case emNodeType::Repeater:
//             return (nodes.size() > 0);
//         case emNodeType::Leaf:
//         default:
//             return false;
//     }
// }


// template<typename TShareData>
// emProcessResult Node<TShareData>::Process(TShareData& share_data)
// {
//     /* 一个节点的执行结果，根据子节点的执行结果判断
//         逆变节点，执行结果的逆转。true变false、false变true
//         重复节点，一般用作树根，无论执行结果如何，每次递归到一定执行
//         选择节点，执行直到遇到第一个执行成功的子节点
//         次序节点，执行结果和 and 是一个效果
//         成功节点，执行结果一定为 true
//      */

//     bool node_result = true;

//     for (auto&& it : m_sub_nodes) {
//         /* 第一次、Running状态下可以active */
//         if (it->second.Type() == emProcessResult::Success || it->second.Type() == emProcessResult::Failed)
//             continue;

//         emProcessResult active_result = m_active_callback(share_data);

//         /* 根据子行为树的执行结果，计算本节点的执行结果 */
//         switch (m_type) {
//             case emNodeType::Inverter:
//                 m_cur_result = !active_result;
//                 break;
//             case emNodeType::Repeater:
//                 m_cur_result |= active_result;
//                 break;
//             case emNodeType::Selector:
//                 m_cur_result = active_result;
//                 /* 执行成功不继续执行 */
//                 if (m_cur_result) break;
//                 break;
//             case emNodeType::Sequences:
//                 m_cur_result &= active_result;
//                 if (!m_cur_result) break;
//                 break;
//             case emNodeType::Successder:
//                 m_cur_result = true;
//                 break;
//             case emNodeType::Leaf:
//                 m_cur_result = active_result;
//                 break;
//             default:
//                 break;
//         }
//     }

//     if (emNodeType::Inverter) {
//         m_cur_result = !m_cur_result;   // 逆转结果
//     }

//     return m_cur_result;
// }





template<typename TShareData>
emProcessResult SequencesNode<TShareData>::Process(TShareData& share_data)
{
    /* 次序节点，所有子节点都执行 Success 才算成功，否则算做 Failed */
    bool this_node_result = true;

    for (auto&& it_node : m_sub_nodes)
    {
        this_node_result |= it_node->second.Process(share_data);
    }
    
    return emProcessResult;
}

template<typename TShareData>
bool SequencesNode<TShareData>::__CheckCanSetNodes(std::map<int, typename Node<TShareData, emNodeType::Sequences>::NodeBase*> nodes)
{
    
    return true;
}
