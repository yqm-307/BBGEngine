#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <bbt/random/Random.hpp>
#include "util/vector/CalcOpt.hpp"
#include "util/testhelper/Helper.hpp"

using namespace util;
using namespace util::pos;
using namespace util::test;
using namespace util::vector;

/* 点和向量的空间运算 */
BOOST_AUTO_TEST_SUITE(PointAndVectorTest)

/* 1、根据两点坐标计算向量 */
BOOST_AUTO_TEST_CASE(t_vector_calc_test)
{
    typedef std::pair<Point3, Point3> PointPair;
    typedef TestSample<PointPair> TSample;

    std::vector<TSample> m_samples;
    std::vector<util::vector::Vector3> m_results;
    m_samples.push_back( std::make_pair(Point3{9.18f,   4.77f,  12.53f},Point3{5.4f,  7.69f, 4.0f}));
    m_samples.push_back( std::make_pair(Point3{12.64f,  5.68f,  0.0f},  Point3{3.86f, 7.66f, 0.0f}));
    m_samples.push_back( std::make_pair(Point3{4.0f,    2.0f,   0.94f}, Point3{7.28f, 6.0f,  3.47f}));
    m_samples.push_back( std::make_pair(Point3{1.0f,    2.0f,   5.0f},  Point3{7.0f,  9.0f,  12.0f}));
    m_samples.push_back( std::make_pair(Point3{-1.0f,   2.0f,   5.0f},  Point3{7.0f, -9.0f,  12.0f}));

    m_results.push_back( vector::Vector3{-3.78f, 2.92f,  -8.53f});
    m_results.push_back( vector::Vector3{-8.78f, 1.98f,   0.0f});
    m_results.push_back( vector::Vector3{3.28f,  4.0f,    2.53f});
    m_results.push_back( vector::Vector3{6.0f,   7.0f,    7.0f});
    m_results.push_back( vector::Vector3{8.0f,  -11.0f,  -7.0f});



    util::test::EasyHelper<PointPair> helper(m_samples.size());

    helper.SetSample(m_samples, nullptr);
    helper.SetTestHandler([&](const TSample& sample, uint32_t round){
        Vector3 vec = CalcVectorByPos(sample.m_sample.first, sample.m_sample.second);
        if (!m_results[round - 1].Approx(vec)) {
            return util::test::EasyHelperResult::emFailed;
        }
        return util::test::EasyHelperResult::emOk;
    });

    helper.Start();

    helper.PrintResult();

}


BOOST_AUTO_TEST_SUITE_END()