#define BOOST_TEST_MODULE hashmap test
#include <boost/test/included/unit_test.hpp>
#include <bbt/random/Random.hpp>
#include <bbt/timer/Interval.hpp>
#include "util/hashmap/Hashmap.hpp"
// #include <hash_map>


BOOST_AUTO_TEST_CASE(t_hashmap_create_test)
{
    using namespace game::util::hashmap;
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
    using namespace game::util::hashmap;
    const size_t bucket_num = 6;
    Hashmap<int, std::string, bucket_num> map([=](int key){return key%bucket_num;}, "");

    BOOST_ASSERT(map.Insert(1,"a"));
    BOOST_ASSERT(map.Insert(2,"b"));
    BOOST_ASSERT(!map.Insert(1,"c"));

    auto [_4, isok4] = map.Earse(1);
    BOOST_ASSERT(isok4);
    auto [_5, isok5] = map.Earse(1);
    BOOST_ASSERT(!isok5);
    auto [_6, isok6] = map.Earse(3);
    BOOST_ASSERT(!isok6);

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

}


BOOST_AUTO_TEST_CASE(t_hashmap_rom_test)
{
    using namespace game::util::hashmap;
    bbt::random::mt_random<int, 1, 1000000> rd1; 
    bbt::random::mt_random<int, 1, INT32_MAX> rd2; 
    Hashmap<int, int, 256> map([](int key){return key%256;}, 1);

    int default_value = 1;
    printf("my Hashmap----------------------------\n");
    bbt::timer::interval stopwatch; // 开始计时
    std::set<int> tmp;
    for(int i = 0; i<1000000; i++)
    {
        int key = rd2();
        auto it = tmp.insert(key);
        if(it.second)
        {
            BOOST_ASSERT_MSG(map.Insert(key, default_value), "key repeat!");
        }
    }
    printf("插入1w个数据到hashmap中，耗时: %ldms\n", stopwatch.intervalnow());

    stopwatch.recycle();
    std::vector<int> keys(tmp.begin(), tmp.end());
    // 随机访问 10w次
    for(int i = 0; i<1000000; i++)
    {
        int idx = rd2() % keys.size();
        auto it = map.Find(keys[idx]);
        BOOST_ASSERT_MSG(it.second, "not found key!");
    }
    printf("Hashmap搜索100w次，耗时 %ldms\n", stopwatch.intervalnow());

}

BOOST_AUTO_TEST_CASE(t_hashmap_random_func)
{
    using namespace game::util::hashmap;
    bbt::random::mt_random<int, 1, 1000000> rd1; 
    bbt::random::mt_random<int, 1, INT32_MAX> rd2; 
    Hashmap<int, int, 256> map([](int key){return key%256;}, 1);

    bbt::timer::interval stopwatch; // 开始计时
    for(int i = 0; i < 10; ++i)
    {
        
    }

}


// BOOST_AUTO_TEST_CASE(t_std_map_rom_test)
// {
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     std::map<int,int> map;
//     printf("stl map----------------------------");

//     bbt::timer::interval stopwitch;
//     std::set<int> tmp;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             auto [_, isok] = map.insert(std::make_pair(key, 1));
//             BOOST_ASSERT_MSG(isok, "key repeat!");
//         }
//     }
//     printf("插入1w个数据到hashmap中，耗时: %ldms\n", stopwitch.intervalnow());

//     stopwitch.recycle();
//     std::vector<int> keys(tmp.begin(), tmp.end());
//     for(int i = 0; i< 1000000; i++)
//     {
//         int idx = rd1()%keys.size();
//         // bbt::timer::interval sw;
//         auto it = map.find(keys[idx]);
//         // printf("find耗时 %ld ns\n", sw.intervalnow<bbt::timer::clock::ns>());
//         BOOST_ASSERT_MSG(it != map.end(), "not found key!");
//     }
//     printf("stl map搜索100w次，耗时 %ldms \n", stopwitch.intervalnow());
// }

// BOOST_AUTO_TEST_CASE(t_std_hash_map_rom_test)
// {
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     __gnu_cxx::hash_map<int,int> map;
//     printf("stl hash map----------------------------\n");

//     bbt::timer::interval stopwitch;
//     std::set<int> tmp;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             auto [_, isok] = map.insert(std::make_pair(key, 1));
//             BOOST_ASSERT_MSG(isok, "key repeat!");
//         }
//     }
//     printf("插入1w个数据到hashmap中，耗时: %ldms\n", stopwitch.intervalnow());

//     stopwitch.recycle();
//     std::vector<int> keys(tmp.begin(), tmp.end());
//     for(int i = 0; i< 1000000; i++)
//     {
//         int idx = rd1()%keys.size();
//         // bbt::timer::interval sw;
//         auto it = map.find(keys[idx]);
//         // printf("find耗时 %ld ns\n", sw.intervalnow<bbt::timer::clock::ns>());
//         BOOST_ASSERT_MSG(it != map.end(), "not found key!");
//     }
//     printf("stl map搜索100w次，耗时 %ldms \n", stopwitch.intervalnow());
// }

// // hashmap 插入删除效率对比
// BOOST_AUTO_TEST_CASE(t_hashmap_remove_insert)
// {
//     using namespace game::util::hashmap;
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     Hashmap<int, int, 16> hashmap([](int key){return key%16;}, 1);

//     std::set<int> tmp;
//     bbt::timer::interval sw;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             BOOST_ASSERT_MSG(hashmap.Insert(key, 1), "key repeat!");
//         }
//     }
//     printf("插入%ld条数据, 耗时: %ld\n", tmp.size(), sw.intervalnow());
//     sw.recycle();

//     for(auto&& a : tmp)
//     {
//         auto [val, isok] = hashmap.Earse(a);
//         BOOST_ASSERT_MSG(isok, "earse error!");
//     }
//     printf("删除%ld条数据, 耗时: %ld\n", tmp.size(), sw.intervalnow());
// }


// BOOST_AUTO_TEST_CASE(t_map_remove_insert)
// {
//     using namespace game::util::hashmap;
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     std::map<int, int> map;

//     std::set<int> tmp;
//     bbt::timer::interval sw;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             auto [_, isok] = map.insert(std::make_pair(key, 1));
//             BOOST_ASSERT_MSG(isok, "key repeat!");
//         }
//     }
//     printf("插入%ld条数据, 耗时: %ld\n", tmp.size(), sw.intervalnow());
//     sw.recycle();

//     for(auto&& a : tmp)
//     {
//         auto len = map.erase(a);
//         BOOST_ASSERT_MSG(len > 0, "earse error!");
//     }
//     printf("删除%ld条数据, 耗时: %ld\n", tmp.size(), sw.intervalnow());
// }


