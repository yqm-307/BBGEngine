#define BOOST_TEST_INCLUDED
#include <boost/test/included/unit_test.hpp>

// #include "plugin/ecs/aoi/Aoi.hpp"
#include "plugin/ecs/aoi/AoiComponent.hpp"
#include <vector>
#include <bbt/core/random/Random.hpp>

using namespace share::ecs::aoi;

void Box_Init(AABBBox& box, float xmax, float xmin, float ymax, float ymin, float zmax, float zmin)
{
    box.max_x = xmax;
    box.min_x = xmin;
    
    box.max_y = ymax;
    box.min_y = ymin;

    box.max_z = zmax;
    box.min_z = zmin;
}


BOOST_AUTO_TEST_SUITE(AabbTest)

/**
 * aabb盒碰撞检测 
 */
BOOST_AUTO_TEST_CASE(t_aabb_check_test)
{
    using namespace share::ecs::aoi;
    // 重叠检测
    AABBBox a;
    AABBBox b;
    AABBBox c;
    AABBBox d;
    AABBBox e;
    Box_Init(a, 1, 0, 1, 0, 1, 0);
    Box_Init(b, 1, 0, 1, 0, 2, 0);
    Box_Init(c, 3, 0, 3, 0, 3, 0);
    Box_Init(d, 2, 1, 1, 0, 1, 0);
    Box_Init(e, 2, 1, 2, 1, 2, 1);
    bool rlt1 = AABBCheck(a, a);
    BOOST_CHECK_MESSAGE(rlt1, "重叠测试失败");

    bool rlt2 = AABBCheck(a, b);
    BOOST_CHECK(rlt2);

    bool rlt3 = AABBCheck(a, c);
    BOOST_CHECK(rlt3);

    bool rlt4 = AABBCheck(a, d);
    BOOST_CHECK(!rlt4);

    bool rlt5 = AABBCheck(a, e);
    BOOST_CHECK(!rlt5);
}

BOOST_AUTO_TEST_SUITE_END()