#include "EatFoodTree.hpp"
#include <thread>

int main()
{
    EatFoodShareData food_info{
        .m_total_meats = 4,
        .m_total_vegetables = 3,
    };

    EatFoodTree food;

    while (food.Tick(food_info) != emNodeStatus::Failed)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}