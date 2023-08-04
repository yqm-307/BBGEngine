#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>
#include <optional>
#include <array>
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
    Slot _data[size];
    std::atomic_size_t _r_count;
    std::atomic_size_t _w_count;
};

#include "util/lockfree/LockFreeQueueTpl.hpp"


// class TmpVersion
//     :public bbt::templateutil::comparator<uint64_t>
// {
//     typedef bbt::templateutil::comparator<uint64_t> CompItem;
// public:
//     TmpVersion()
//     {
//         this->SetValue(bbt::timer::clock::now<bbt::timer::clock::ns>().time_since_epoch().count());
//     }
//     virtual bool operator==(const CompItem& rval) const override
//     {
//         return ((it_) == (rval.GetValue()));
//     }
//     virtual bool operator>(const CompItem& rval) const override
//     {
//         return ((it_) > (rval.GetValue()));
//     }
// };

// /**
//  * @brief 节点
//  * 
//  * @tparam T 保存元素类型
//  * @tparam VersionType 要求VersionType类在构造时自动生成一个不重复的id，支持比较操作
//  */
// template<typename T, typename VersionType>
// struct LockFreeQueueSlot
// {
//     T m_item;
//     VersionType m_version;
// };


// template<typename T, size_t Num>
// class LockFreeQueue
// {
// public:
//     explicit LockFreeQueue();
//     ~LockFreeQueue();

//     T Pop();
//     void Push();
//     size_t Size();
//     bool Empty();
//     void Clear();
// private:
//     std::array<LockFreeQueueSlot<T, TmpVersion>, Num> m_arr;
// };


}