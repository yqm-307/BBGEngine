#define BOOST_TEST_INCLUDED
#include <boost/test/included/unit_test.hpp>
#include <thread>
#include <bbt/core/random/Random.hpp>
#include <bbt/core/timer/Interval.hpp>
#include "util/lockfree/LockFreeQueue.hpp"

BOOST_AUTO_TEST_SUITE(LockFreeTest)

BOOST_AUTO_TEST_CASE(t_lockfree_test)
{
    const int nthread = 20;
    std::vector<std::thread*> threads(nthread, nullptr);

    // 无锁队列
    util::lockfree::Queue<int, 1000000> lockfree_queue;
    std::atomic_int64_t consume_count{0};
    std::atomic_int64_t produce_count{0};


    for(int i = 0; i < nthread; ++i)
    {
        if( i%2 == 0 ) 
            // Producter 生产4s
            threads[i] = new std::thread([&lockfree_queue, &produce_count](){
                auto timeout = bbt::timer::clock::nowAfter(bbt::timer::clock::ms(4000));
                while(!bbt::timer::clock::expired<bbt::timer::clock::ms>(timeout))
                {
                    if(lockfree_queue.Push(1))
                        ++produce_count;
                }
            });
        else 
            // Consumer 消费5s
            threads[i] = new std::thread([&lockfree_queue, &consume_count](){
                auto timeout = bbt::timer::clock::nowAfter(bbt::timer::clock::ms(5000));
                while(!bbt::timer::clock::expired<bbt::timer::clock::ms>(timeout))
                {
                    int a = 0;
                    if(lockfree_queue.Pop(a))
                        ++consume_count;

                }
            });
    }

    for(int i = 0; i < nthread; ++i)
    {
        threads[i]->join();
    }


    BOOST_CHECK(produce_count > 0 && consume_count > 0);
    BOOST_CHECK_EQUAL(produce_count.load(), consume_count.load());
    BOOST_TEST_MESSAGE("produce count: " << produce_count.load() << "\tconsume count: " << consume_count.load());

}


BOOST_AUTO_TEST_SUITE_END()