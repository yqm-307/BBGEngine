// #include <bbt/random/Random.hpp>
// #include <bbt/timer/Interval.hpp>
// #include "util/hashmap/Hashmap.hpp"
// #include <hash_map>
// #include <set>
// #include <unordered_map>

// void test1()
// {
//     using namespace game::util::hashmap;
//     bbt::random::mt_random<int, 1, 1000000> rd1; 
//     bbt::random::mt_random<int, 1, INT32_MAX> rd2; 
//     Hashmap<int, int, 256> map([](int key){return key%256;}, 1);

//     int default_value = 1;
//     printf("my Hashmap----------------------------\n");
//     bbt::timer::interval stopwatch; // 开始计时
//     std::set<int> tmp;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd2();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//         }
//     }
//     printf("插入1w个数据到hashmap中，耗时: %ldms\n", stopwatch.intervalnow());
//     srand((uint32_t)time(NULL));
//     stopwatch.recycle();
//     std::vector<int> keys(tmp.begin(), tmp.end());
//     // 随机访问 10w次
//     for(int i = 0; i<1000000; i++)
//     {
//         int idx = rand() % keys.size();
//         auto it = map.Find(keys[idx]);
//     }
//     printf("Hashmap搜索100w次，耗时 %ldms\n", stopwatch.intervalnow());
// }

// void test2()
// {
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     std::map<int,int> map;
//     printf("stl map----------------------------\n");

//     bbt::timer::interval stopwitch;
//     std::set<int> tmp;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             auto [_, isok] = map.insert(std::make_pair(key, 1));
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
//     }
//     printf("stl map搜索100w次，耗时 %ldms \n", stopwitch.intervalnow());
// }

// void test3()
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
//     }
//     printf("stl map搜索100w次，耗时 %ldms \n", stopwitch.intervalnow());
// }

// void test4()
// {
//     bbt::random::mt_random<int, 1, INT32_MAX> rd1;
//     std::unordered_map<int,int> map;
//     // __gnu_cxx::hash_map<int,int> map;
//     printf("stl unordered map----------------------------\n");

//     bbt::timer::interval stopwitch;
//     std::set<int> tmp;
//     for(int i = 0; i<1000000; i++)
//     {
//         int key = rd1();
//         auto it = tmp.insert(key);
//         if(it.second)
//         {
//             auto [_, isok] = map.insert(std::make_pair(key, 1));
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
//     }
//     printf("stl map搜索100w次，耗时 %ldms \n", stopwitch.intervalnow());
// }

// int main()
// {
//     test1();
//     test2();
//     test3();
//     test4();
// }