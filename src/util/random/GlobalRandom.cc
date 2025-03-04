#include "util/random/GlobalRandom.hpp"
#include "util/assert/Assert.hpp"

namespace util
{

time_t Random::m_random_seed = 0;
bbt::random::mt_random<int64_t, INT64_MIN, INT64_MAX> Random::m_rand = bbt::random::mt_random<int64_t, INT64_MIN, INT64_MAX>();

void Random::SetSeed(time_t seed)
{
    m_random_seed = seed;
    m_rand.SetSeed(seed);
}

time_t Random::GetSeed()
{
    return m_random_seed;
}

int64_t Random::Rand()
{
    DebugAssert(m_random_seed != 0);
    return m_rand();
}


}// namespace end