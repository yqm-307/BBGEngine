#pragma once
#include "util/other/Math.hpp"
#include "util/random/GlobalRandom.hpp"
#include "util/assert/Assert.hpp"

namespace util::other
{

template<typename T>
bool Math::Derange(std::vector<T>& vec)
{
    size_t len = vec.size();
    if(len <= 0)
        return false;

    for(size_t i = 0; i < len; ++i)
    {
        size_t swp_target_idx = (::abs(util::Random::Rand())) % len;
        DebugAssert(swp_target_idx >= 0 && swp_target_idx < len);
        std::swap(vec[i], vec[swp_target_idx]);
    }
    return true;
}

}// namespace end