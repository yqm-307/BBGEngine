#pragma once
#include "BehaviorTree.hpp"
#include <string>
#include <map>

// 吃的菜的类型
enum emFoodType
{
    EM_MEAT_DISH = 1,       // 肉菜
    EM_VEGETABLE_DISHES = 2, // 素菜
};

// 吃不同的菜消耗的时间
std::map<emFoodType, int> g_eat_dish_cost_times_map = {
    {EM_MEAT_DISH, 4},
    {EM_VEGETABLE_DISHES, 2},
};

struct EatFoodShareData
{
    emFoodType  m_cur_eat_food;             // 当前吃的哪道菜
    int         m_cur_food_cost_times;      // 当前吃的菜已经吃了多久了
    int         m_cost_total_times;         // 总共吃了多久了
};

class EatFoodTree:
    public BehaviorTree<EatFoodShareData>
{
public:
    EatFoodTree();
    ~EatFoodTree();
private:
};

EatFoodTree::EatFoodTree(/* args */)
{
}

EatFoodTree::~EatFoodTree()
{
}

