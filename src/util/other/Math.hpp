#pragma once
#include <cmath>
#include <vector>


namespace util::other
{

class Math
{
public:

/**
 * @brief 将vec随机打乱
 * 
 * @tparam T 
 * @param vec 
 * @return true 打乱成功，vec为结果
 * @return false 打乱失败，vec不变
 */
template<typename T>
static bool Derange(std::vector<T>& vec);
 
};

}// namespace end

#include "util/other/MathTpl.hpp"