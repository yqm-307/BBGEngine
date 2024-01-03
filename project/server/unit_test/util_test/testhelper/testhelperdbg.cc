#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "util/testhelper/Helper.hpp"
#include <bbt/random/Random.hpp>
#include <bbt/uuid/EasyID.hpp>

#define INIT_SAMPLE(v, t) util::test::TestSample<t>{.m_sample = v,.m_sample_id = v}

BOOST_AUTO_TEST_SUITE(TestHelperDebug)

BOOST_AUTO_TEST_CASE(t_helper_debug_1)
{
    bbt::uuid::EasyID<bbt::uuid::EM_AUTO_INCREMENT, 1> IDGenerator;
    bbt::random::mt_random myrd;

    util::test::EasyHelper<int> helper(100);

    std::vector<util::test::TestSample<int>> fixed_sample;
    // fixed_sample.insert(INIT_SAMPLE(5, int));
    // util::test::TestSample<int>{.m_sample = 1, .m_sample_id = 1};
    fixed_sample.push_back(util::test::TestSample<int>(1));
    fixed_sample.push_back(util::test::TestSample<int>(2));
    fixed_sample.push_back(util::test::TestSample<int>(3));
    fixed_sample.push_back(util::test::TestSample<int>(4));
    fixed_sample.push_back(util::test::TestSample<int>(5));

    helper.SetSample(std::move(fixed_sample), [&myrd, &IDGenerator](int32_t round){ 
        util::test::TestSample<int> sample(round);
        return sample;
    });

    helper.SetHandler([](const util::test::TestSample<int>& sample){
        int rlt = sample.m_sample % 3;
        if (rlt == 0) {
            return util::test::EasyHelperResult::emOk;
        } else if (rlt == 1) {
            return util::test::EasyHelperResult::emFailed;
        } else {
            return util::test::EasyHelperResult::emWarning;
        }
    }
    );

    helper.Start();

}

BOOST_AUTO_TEST_SUITE_END()