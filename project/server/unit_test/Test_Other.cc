#define BOOST_TEST_INCLUDED
#include <boost/test/included/unit_test.hpp>
#include <thread>
#include <bbt/random/Random.hpp>
#include <bbt/timer/Interval.hpp>
#include "util/other/Math.hpp"

template<typename T>
std::map<T, int> Stat(const std::vector<T>& vec)
{
    std::map<T, int> stat;
    for(auto&& it : vec)
    {
        if(stat.end() == stat.find(it))
            stat[it] = 1;
        else
            stat[it]++;
    }
    return stat;
}

template<typename T>
void CheckDerangeIsOk(const std::vector<T>& initial_vec, const std::vector<T>& rlt_vec)
{
    std::map<int, int> stat1 = Stat<T>(initial_vec);
    std::map<int, int> stat2 = Stat<T>(rlt_vec);

    for(auto&& it : stat1)
    {
        BOOST_ASSERT(stat2[it.first] == it.second);
    }

}


BOOST_AUTO_TEST_CASE(t_math_random)
{
    game::util::Random::SetSeed(bbt::timer::clock::now().time_since_epoch().count());
    std::vector<int> vec{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> vec1 = vec;
    BOOST_ASSERT(game::util::other::Math::Derange(vec1));
    CheckDerangeIsOk<int>(vec, vec1);

    std::vector<int> vec2{};
    BOOST_ASSERT(!game::util::other::Math::Derange(vec2));


}