#define BOOST_TEST_MODULE aabb box test
#include <boost/test/included/unit_test.hpp>
#include "share/ecs/entity/aoi/Aoi.hpp"
#include "share/ecs/entity/player/Player.hpp"
#include <vector>
#include <bbt/random/Random.hpp>

using namespace game::share::ecs::entity::aoi;

void Box_Init(AABBBox& box, float xmax, float xmin, float ymax, float ymin, float zmax, float zmin)
{
    box.max_x = xmax;
    box.min_x = xmin;
    
    box.max_y = ymax;
    box.min_y = ymin;

    box.max_z = zmax;
    box.min_z = zmin;
}


/**
 * aabb盒碰撞检测 
 */
BOOST_AUTO_TEST_CASE(t_aabb_check_test)
{
    using namespace game::share::ecs::entity::aoi;
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
    Box_Init(e, 1, 1, 1, 1, 1, 1);
    bool rlt1 = AABBCheck(a, a);
    BOOST_ASSERT_MSG(rlt1, "重叠测试失败");

    bool rlt2 = AABBCheck(a, b);
    BOOST_ASSERT(rlt2);

    bool rlt3 = AABBCheck(a, c);
    BOOST_ASSERT(rlt3);

    bool rlt4 = AABBCheck(a, d);
    BOOST_ASSERT(!rlt4);

    bool rlt5 = AABBCheck(a, e);
    BOOST_ASSERT(!rlt5);
}