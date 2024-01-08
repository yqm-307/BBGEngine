#pragma once
#include "Node.hpp"


template<typename TShareData>
bool Node<TShareData>::__CheckCanSetNodes(std::map<int, Node*> nodes)
{
    switch (m_type) {
        /* 存在至少一个节点 */
        case emNodeType::Sequences:
        case emNodeType::Selector:
        case emNodeType::Successder:
        case emNodeType::Inverter:
        case emNodeType::Repeater:
            return (nodes.size() > 0);
        case emNodeType::Leaf:
        default:
            return false;
    }
}
