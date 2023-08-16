#pragma once
#include <cmath>
#include <vector>


namespace game::util::other
{




/* 打乱 vec，使用时需要注意 */
template<typename T>
static void Derange(std::vector<T> vec);

}// namespace end

#include "util/other/mathTpl.hpp"