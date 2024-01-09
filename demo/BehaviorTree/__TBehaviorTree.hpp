#pragma once
#include "BehaviorTree.hpp"

template<typename TShareData>
BehaviorTree<TShareData>::BehaviorTree()
{
}

template<typename TShareData>
BehaviorTree<TShareData>::~BehaviorTree()
{
}

template<typename TShareData>
void BehaviorTree<TShareData>::SetTree(TNode* tree_root)
{
    m_root = tree_root;
}

template<typename TShareData>
emNodeStatus BehaviorTree<TShareData>::Tick(TShareData& data)
{
    return m_root->Process(data);
}
