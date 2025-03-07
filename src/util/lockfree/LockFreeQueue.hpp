#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>
#include <optional>
#include <array>
#include <vector>
#include <bbt/core/templateutil/Comparator.hpp>
#include <bbt/core/timer/Clock.hpp>

namespace util::lockfree
{

/**
 *  基于C++ atomic 实现的无锁队列，涉及到 memory_order ，因此下面注释会详细解释
 *  1、用作计数的atomic，只需要保证原子性即可。因此使用的memory_order是 memory_order_relaxed
 *  2、使用 memory_order_release 要在当前的逻辑之后，之前有读操作要用 memory_order_acquire
 */


template <typename T, size_t size> 
class Queue {
    static_assert(size > 2, "Buffer size must be bigger than 2");

public:
    Queue();

    /**
     * @brief 向队尾追加一个元素
     * 
     * @param[in] element 入队元素
     * @return true 
     * @return false 
     */
    bool Push(const T &element);

    /**
     * @brief 从队尾弹出一个元素
     * 
     * @param[out] element 出队元素
     * @return true 
     * @return false 
     */
    bool Pop(T &element);

private:
    struct Slot {
        T obj;
        std::atomic_size_t pop_count;   // 当前槽已经pop的次数
        std::atomic_size_t push_count;  // 当前槽已经push的次数

        Slot() : pop_count(0U), push_count(0U) {}
    };

private:
    std::vector<Slot> m_data;
    std::atomic_size_t m_read_count;    // 已经读的总数，取余可获得index
    std::atomic_size_t m_write_count;   // 已经写的总数，取余可活动index
};

}


#include "util/lockfree/LockFreeQueueTpl.hpp"