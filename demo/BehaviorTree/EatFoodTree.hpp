#pragma once
#include "BehaviorTree.hpp"
#include <string>
#include <map>

// 吃的菜的类型
enum emFoodType
{
    EM_DEFAULT = 0,
    EM_MEAT_DISH = 1,       // 肉菜
    EM_VEGETABLE_DISHES = 2, // 素菜
};

// 吃不同的菜消耗的时间
static std::map<emFoodType, int> g_eat_dish_cost_times_map = {
    {EM_MEAT_DISH, 4},
    {EM_VEGETABLE_DISHES, 2},
};

struct EatFoodShareData
{
    const int   m_total_meats;
    const int   m_total_vegetables;

    emFoodType  m_cur_eat_food{emFoodType::EM_DEFAULT};             // 当前吃的哪道菜
    int         m_cur_food_cost_times{0};      // 当前吃的菜已经吃了多久了

    int         m_remain_meats{m_total_meats};
    int         m_remain_vegetanles{m_total_vegetables};
};


/*
    假设场景，小明吃饭，有素菜和荤菜。
        （1）小明喜欢吃荤菜，有荤菜先吃荤菜；
        （2）吃荤菜4分钟；
        （3）吃素菜2分钟；

    行为树如下：
    |--@AiRoot(RepeatUtilFail)
            |--@EatDish(Sequences)
                    |--@EatMeat(RepeatUtilFail)
                            |--@CheckHasMeatDish(Conditional)
                            |--@DoEatMeat(Action)
                    |--@EatVegetableDish(RepeatUtilFail)
                            |--@CheckHasVegetableDish(Conditional)
                            |--@DoEatVegetableDish(Action)
            |--@LeaveTable(Action)

 */

class EatFoodTree:
    public BehaviorTree<EatFoodShareData>
{
public:
    EatFoodTree();
    ~EatFoodTree();
private:
    static emNodeStatus DoEatMeat(EatFoodShareData& share);
    static emNodeStatus DoEatVagetable(EatFoodShareData& share);
    static emNodeStatus DoLeaveTable(EatFoodShareData& share);

    static emNodeStatus CondHasMeat(EatFoodShareData& share);
    static emNodeStatus CondHasValgetable(EatFoodShareData& share);
private:
    RepeatUntilFailNode<EatFoodShareData>*  m_root;
    SequencesNode<EatFoodShareData>*        m_eat_dish;

    RepeatUntilFailNode<EatFoodShareData>*  m_eat_meat;
    RepeatUntilFailNode<EatFoodShareData>*  m_eat_vegetable;

    ConditionalNode<EatFoodShareData>*      m_check_can_eat_meat;
    ConditionalNode<EatFoodShareData>*      m_check_can_eat_vegetable;

    ActionNode<EatFoodShareData>*           m_do_eat_meat;
    ActionNode<EatFoodShareData>*           m_do_eat_vegetable;
    ActionNode<EatFoodShareData>*           m_do_leave_table;
};

