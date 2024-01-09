#include "EatFoodTree.hpp"


// std::map<emFoodType, int> g_eat_dish_cost_times_map = {
//     {EM_MEAT_DISH, 4},
//     {EM_VEGETABLE_DISHES, 2},
// };


EatFoodTree::EatFoodTree()
{
    m_root = new RepeatUntilFailNode<EatFoodShareData>();
    {
        m_eat_dish = new SequencesNode<EatFoodShareData>();
        {
            m_eat_meat = new RepeatUntilFailNode<EatFoodShareData>();
            {
                m_check_can_eat_meat = new ConditionalNode<EatFoodShareData>();
                m_check_can_eat_meat->SetCondExpr(EatFoodTree::CondHasMeat);
                m_do_eat_meat = new ActionNode<EatFoodShareData>();
                m_do_eat_meat->SetActive(EatFoodTree::DoEatMeat);
            }

            m_eat_vegetable = new RepeatUntilFailNode<EatFoodShareData>();
            {
                m_check_can_eat_vegetable = new ConditionalNode<EatFoodShareData>();
                m_check_can_eat_vegetable->SetCondExpr(EatFoodTree::CondHasValgetable);
                m_do_eat_vegetable = new ActionNode<EatFoodShareData>();
                m_do_eat_vegetable->SetActive(EatFoodTree::DoEatVagetable);
            }
        }

        m_do_leave_table = new ActionNode<EatFoodShareData>();
        m_do_leave_table->SetActive(EatFoodTree::DoLeaveTable);
    }

    m_root->Init({
        {1, m_eat_dish},
        {2, m_do_leave_table},
    });

    m_eat_dish->Init({
        {1, m_eat_meat},
        {2, m_eat_vegetable},
    });

    m_eat_meat->Init({
        {1, m_check_can_eat_meat},
        {2, m_do_eat_meat},
    });

    m_eat_vegetable->Init({
        {1, m_check_can_eat_vegetable},
        {2, m_do_eat_vegetable},
    });
    

    SetTree(m_root);
}

EatFoodTree::~EatFoodTree()
{

}


emNodeStatus EatFoodTree::DoEatMeat(EatFoodShareData& share)
{
    if (share.m_remain_meats <= 0) {
        return emNodeStatus::Failed;
    }

    if (share.m_cur_eat_food != emFoodType::EM_MEAT_DISH) {
        share.m_cur_food_cost_times = 0;
        share.m_cur_eat_food = emFoodType::EM_MEAT_DISH;
    }

    share.m_cur_food_cost_times++;
    printf("[eat meat] times:%d\n", share.m_cur_food_cost_times);

    if (share.m_cur_food_cost_times >= g_eat_dish_cost_times_map[emFoodType::EM_MEAT_DISH]) {
        share.m_remain_meats--;
        share.m_cur_food_cost_times = 0;
        return emNodeStatus::Success;
    }

    return emNodeStatus::Running;
}

emNodeStatus EatFoodTree::DoEatVagetable(EatFoodShareData& share)
{
    if (share.m_remain_vegetanles <= 0) {
        return emNodeStatus::Failed;
    }

    if (share.m_cur_eat_food != emFoodType::EM_VEGETABLE_DISHES) {
        share.m_cur_food_cost_times = 0;
        share.m_cur_eat_food = emFoodType::EM_VEGETABLE_DISHES;
    }

    share.m_cur_food_cost_times++;
    printf("[eat vegetable] times:%d\n", share.m_cur_food_cost_times);

    if (share.m_cur_food_cost_times >= g_eat_dish_cost_times_map[emFoodType::EM_VEGETABLE_DISHES]) {
        share.m_remain_vegetanles--;
        share.m_cur_food_cost_times = 0;
        return emNodeStatus::Success;
    }

    return emNodeStatus::Running;
}

emNodeStatus EatFoodTree::DoLeaveTable(EatFoodShareData& share)
{
    printf("[EatFoodTree::DoLeaveTable]\n");
    return emNodeStatus::Success;
}

emNodeStatus EatFoodTree::CondHasMeat(EatFoodShareData& share)
{
    if (share.m_remain_meats <= 0)
        return emNodeStatus::Failed;
    else
        return emNodeStatus::Success;
}

emNodeStatus EatFoodTree::CondHasValgetable(EatFoodShareData& share)
{
    if (share.m_remain_vegetanles <= 0)
        return emNodeStatus::Failed;
    else
        return emNodeStatus::Success;
}