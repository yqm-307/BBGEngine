#pragma once
#include "Node.hpp"

template<typename TShareData>
class BehaviorTree
{
public:
    typedef Node<TShareData> TNode;

    BehaviorTree();
    virtual ~BehaviorTree() = 0;


    /**
     * @brief 驱动行为树
     * 
     * @return ProcessResult 
     */
    virtual emNodeStatus Tick(TShareData& data) final;
protected:
    virtual void SetTree(TNode* tree_root) final;
private:
    TNode* m_root;
};


#include "__TBehaviorTree.hpp"