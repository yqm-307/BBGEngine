#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>
#include <optional>
#include <array>
#include <vector>
#include <bbt/templateutil/Comparator.hpp>
#include <bbt/timer/Clock.hpp>

namespace game::util::lockfree
{

/*************************** TYPES ****************************/

template <typename T, size_t size> 
class Queue {
    static_assert(std::is_trivial<T>::value, "The type T must be trivial");
    static_assert(size > 2, "Buffer size must be bigger than 2");

public:
    Queue();

    /**
     * @brief Adds an element into the queue.
     * @param[in] element
     * @retval Operation success
     */
    bool Push(const T &element);

    /**
     * @brief Removes an element from the queue.
     * @param[out] element
     * @retval Operation success
     */
    bool Pop(T &element);

    /**
     * @brief Removes an element from the queue.
     * @retval Either the element or nothing
     */
    std::optional<T> PopOptional();

private:
    struct Slot {
        T val;
        std::atomic_size_t pop_count;
        std::atomic_size_t push_count;

        Slot() : pop_count(0U), push_count(0U) {}
    };

private:
    std::vector<Slot> m_data;
    std::atomic_size_t m_read_count;
    std::atomic_size_t m_write_count;
};

}


#include "util/lockfree/LockFreeQueueTpl.hpp"