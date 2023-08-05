#define BOOST_TEST_MODULE lockfree test
#include <boost/test/included/unit_test.hpp>
#include <thread>
#include <bbt/random/Random.hpp>
#include <bbt/timer/Interval.hpp>
#include "util/lockfree/LockFreeQueue.hpp"


BOOST_AUTO_TEST_CASE(t_lockfree_test)
{
    const int nthread = 20;
    std::vector<std::thread*> threads(nthread, nullptr);

    // 无锁队列
    game::util::lockfree::Queue<int, 1000000> lockfree_queue;


    for(int i = 0; i < nthread; ++i)
    {
        if( nthread%2 )
            threads[i] = new std::thread([&lockfree_queue](){
                auto timeout = bbt::timer::clock::nowAfter(bbt::timer::clock::ms(5000));
                while(!bbt::timer::clock::expired<bbt::timer::clock::ms>(timeout))
                {
                    lockfree_queue.Push(1);
                }
            });
        else
            threads[i] = new std::thread([&lockfree_queue](){
                auto timeout = bbt::timer::clock::nowAfter(bbt::timer::clock::ms(5000));
                while(!bbt::timer::clock::expired<bbt::timer::clock::ms>(timeout))
                {
                    lockfree_queue.PopOptional();
                }
            });
    }

    for(int i = 0; i < nthread; ++i)
    {
        threads[i]->join();
    }

}