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

struct VecAndPoints
{
    Point3 beg;
    Point3 end;
    Vector3 vec;
};

static const std::vector<VecAndPoints> g_vec_points = {
    VecAndPoints{.beg = {9.18f,   4.77f,  12.53f},  .end = {5.4f,   7.69f,  4.0f},  .vec = {-3.78f, 2.92f,  -8.53f}},
    VecAndPoints{.beg = {12.64f,  5.68f,  0.0f},    .end = {3.86f,  7.66f,  0.0f},  .vec = {-8.78f, 1.98f,   0.0f}},
    VecAndPoints{.beg = {4.0f,    2.0f,   0.94f},   .end = {7.28f,  6.0f,   3.47f}, .vec = {3.28f,  4.0f,    2.53f}},
    VecAndPoints{.beg = {1.0f,    2.0f,   5.0f},    .end = {7.0f,   9.0f,   12.0f}, .vec = {6.0f,   7.0f,    7.0f}},
    VecAndPoints{.beg = {-1.0f,   2.0f,   5.0f},    .end = {7.0f,   -9.0f,  12.0f}, .vec = {8.0f,  -11.0f,   7.0f}},
    VecAndPoints{.beg = {8.0f,    12.0f,  -5.0f},   .end = {-12.0f, -2.0f,  0.0f},  .vec = {-20.0f,-14.0f,   5.0f}},
};

/* 点和向量的空间运算 */
BOOST_AUTO_TEST_SUITE(PointAndVectorTest)

/* 1、根据两点坐标计算向量 */
BOOST_AUTO_TEST_CASE(t_vector_calc_test)
{
    typedef std::pair<Point3, Point3> PointPair;
    typedef TestSample<PointPair> TSample;

    std::vector<TSample> m_samples;
    std::vector<util::vector::Vector3> m_results;
    m_samples.push_back( std::make_pair(g_vec_points[0].beg, g_vec_points[0].end));
    m_samples.push_back( std::make_pair(g_vec_points[1].beg, g_vec_points[1].end));
    m_samples.push_back( std::make_pair(g_vec_points[2].beg, g_vec_points[2].end));
    m_samples.push_back( std::make_pair(g_vec_points[3].beg, g_vec_points[3].end));
    m_samples.push_back( std::make_pair(g_vec_points[4].beg, g_vec_points[4].end));
    m_samples.push_back( std::make_pair(g_vec_points[5].beg, g_vec_points[5].end));


    m_results.push_back( g_vec_points[0].vec);
    m_results.push_back( g_vec_points[1].vec);
    m_results.push_back( g_vec_points[2].vec);
    m_results.push_back( g_vec_points[3].vec);
    m_results.push_back( g_vec_points[4].vec);
    m_results.push_back( g_vec_points[5].vec);



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

    int succ_num = helper.GetCountByResultType(util::test::EasyHelperResult::emOk);
    BOOST_ASSERT( succ_num == m_samples.size() );

}


/* 2、坐标点根据向量平移 */
BOOST_AUTO_TEST_CASE(t_point_offset_by_vector)
{
    typedef std::tuple<Point3, Vector3> PointAndVector;
    typedef TestSample<PointAndVector> TSample;

    std::vector<TSample>        samples;
    std::vector<Point3>         results;

    samples.push_back(PointAndVector{g_vec_points[0].beg, g_vec_points[0].vec});
    samples.push_back(PointAndVector{g_vec_points[1].beg, g_vec_points[1].vec});
    samples.push_back(PointAndVector{g_vec_points[2].beg, g_vec_points[2].vec});
    samples.push_back(PointAndVector{g_vec_points[3].beg, g_vec_points[3].vec});
    samples.push_back(PointAndVector{g_vec_points[4].beg, g_vec_points[4].vec});
    samples.push_back(PointAndVector{g_vec_points[5].beg, g_vec_points[5].vec});

    results.push_back(g_vec_points[0].end);
    results.push_back(g_vec_points[1].end);
    results.push_back(g_vec_points[2].end);
    results.push_back(g_vec_points[3].end);
    results.push_back(g_vec_points[4].end);
    results.push_back(g_vec_points[5].end);

    EasyHelper<PointAndVector> helper(samples.size());
    helper.SetSample(samples, nullptr);
    helper.SetHandler([&](const TSample& sample, uint32_t round){
        Point3 end = CalcPos3ByVector(std::get<0>(sample.m_sample), std::get<1>(sample.m_sample));
        if (end != results[round - 1]) {
            return EasyHelperResult::emFailed;
        }
        return EasyHelperResult::emOk;
    });

    helper.Start();
    helper.PrintResult();

    BOOST_ASSERT( results.size() == helper.GetCountByResultType(EasyHelperResult::emOk) );
}


BOOST_AUTO_TEST_SUITE_END()