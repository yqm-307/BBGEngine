#pragma once
#include <bbt/core/util/Assert.hpp>
#include <bbt/core/crypto/Random.hpp>

#include "util/other/Math.hpp"

namespace util::other
{

template<typename T>
bool Math::Derange(std::vector<T>& vec)
{
    static bbt::core::crypto::mt_random rand;
    size_t len = vec.size();
    if(len <= 0)
        return false;

    for(size_t i = 0; i < len; ++i)
    {
        size_t swp_target_idx = (::abs((int64_t)rand())) % len;
        DebugAssert(swp_target_idx >= 0 && swp_target_idx < len);
        std::swap(vec[i], vec[swp_target_idx]);
    }
    return true;
}

}// namespace end