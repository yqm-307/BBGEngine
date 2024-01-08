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
emProcessResult BehaviorTree<TShareData>::Tick()
{
    while ()
    {
    }
    
}
