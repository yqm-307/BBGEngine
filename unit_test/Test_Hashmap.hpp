#define BOOST_TEST_INCLUDED
#include <boost/test/included/unit_test.hpp>
#include <bbt/core/random/Random.hpp>
#include <bbt/core/timer/Interval.hpp>
#include "util/hashmap/Hashmap.hpp"
// #include <hash_map>

BOOST_AUTO_TEST_SUITE(HashMapTest)

BOOST_AUTO_TEST_CASE(t_hashmap_create_test)
{
    using namespace util::hashmap;
    // Hashmap<int, int, -1> map([](int key){return key%4;});   // error
    // Hashmap<int, int, 0> map0([](int key){return key%0;});   // error
    Hashmap<int, int, 4> map4([](int key){return key%4;}, 1);
    Hashmap<int, int, 5> map5([](int key){return key%5;}, 1);
    Hashmap<int, int, 15> map15([](int key){return key%15;}, 1);
    Hashmap<int, int, 16> map16([](int key){return key%16;}, 1);
    // Hashmap<int, int, 17> map17([](int key){return key%17;}); // error
}

// 功能测试
BOOST_AUTO_TEST_CASE(t_hashmap_base_test)
{
    using namespace util::hashmap;
    const size_t bucket_num = 6;
    Hashmap<int, std::string, bucket_num> map([=](int key){return key%bucket_num;}, "");

    BOOST_CHECK(map.Insert(1,"a"));
    BOOST_CHECK(map.Insert(2,"b"));
    BOOST_CHECK(!map.Insert(1,"c"));

    auto [_4, isok4] = map.Earse(1);
    BOOST_CHECK(isok4);
    auto [_5, isok5] = map.Earse(1);
    BOOST_CHECK(!isok5);
    auto [_6, isok6] = map.Earse(3);
    BOOST_CHECK(!isok6);

    BOOST_CHECK_EQUAL( map.Size(), 1);

    BOOST_CHECK_EQUAL( map.BucketSize(), bucket_num);

    Hashmap<int, int, 10> ma2p([=](int key){return key%10;}, 0,
    {
        {1, 10},
        {2, 10},
        {3, 10},
        {4, 10},
        {5, 10},
        {6, 10},
    }
    );


    Hashmap<int,int,8> map3([=](int key){return key%10;}, 0);
    map3.Insert({
        {1,1},
        {2,1},
        {3,1},
    });

    map3.Clear();
    BOOST_CHECK(map3.Size() == 0);
    map3.Insert({
        {1,1},
        {2,1},
        {3,1},
    });
    BOOST_CHECK(map3.Size() == 3);
    map3.Clear();

    BOOST_CHECK(map3.Insert(1, 1) == true);
    BOOST_CHECK(map3.Insert(1, 2) == false);
    BOOST_CHECK(map3.Size() == 1);


}


BOOST_AUTO_TEST_CASE(t_hashmap_rom_test)
{
    using namespace util::hashmap;
    bbt::random::mt_random<int, 1, 100000> rd1; 
    bbt::random::mt_random<int, 1, INT32_MAX> rd2; 
    Hashmap<int, int, 256> map([](int key){return key%256;}, 1);

    int default_value = 1;
    // printf("my Hashmap----------------------------\n");
    bbt::timer::interval stopwatch; // 开始计时
    std::set<int> tmp;
    for(int i = 0; i<100000; i++)
    {
        int key = rd2();
        auto it = tmp.insert(key);
        if(it.second)
        {
            BOOST_CHECK_MESSAGE(map.Insert(key, default_value), "key repeat!");
        }
    }
    BOOST_TEST_MESSAGE("插入10w个数据到hashmap中，耗时: " << stopwatch.intervalnow() << "ms");

    stopwatch.recycle();
    std::vector<int> keys(tmp.begin(), tmp.end());
    // 随机访问 10w次
    for(int i = 0; i<100000; i++)
    {
        int idx = rd2() % keys.size();
        auto it = map.Find(keys[idx]);
        BOOST_CHECK_MESSAGE(it.second, "not found key!");
    }
    BOOST_TEST_MESSAGE("Hashmap搜索100w次，耗时: " << stopwatch.intervalnow() << "ms");

}

BOOST_AUTO_TEST_CASE(t_hashmap_random_func)
{
    using namespace util::hashmap;
    bbt::random::mt_random<int, 1, 100000> rd1; 
    bbt::random::mt_random<int, 1, INT32_MAX> rd2; 
    Hashmap<int, int, 256> map([](int key){return key%256;}, 1);

    bbt::timer::interval stopwatch; // 开始计时
    for(int i = 0; i < 10; ++i)
    {
        // TODO
    }
}

BOOST_AUTO_TEST_SUITE_END()