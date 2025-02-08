#include "util/lockfree/LockFreeQueue.hpp"
namespace util::lockfree
{



template <typename T, size_t size>
Queue<T, size>::Queue()
    :m_data(size),
    m_read_count(0U),
    m_write_count(0U) 
{}

template <typename T, size_t size> 
bool Queue<T, size>::Push(const T &element) 
{
    /* 原子的获得已经写的对象总个数 */
    size_t total_write_count = m_write_count.load(std::memory_order_relaxed);

    while (true) {
        /* 此轮我们写入的数组下标 */
        const size_t index = total_write_count % size;
        const size_t slot_push_count = m_data[index].push_count.load(std::memory_order_acquire);
        const size_t slot_pop_count = m_data[index].pop_count.load(std::memory_order_relaxed);

        /* 说明这个下标不可以写 */
        if (slot_push_count > slot_pop_count) 
            return false;

        /* 是否已经被写过了 */ 
        const size_t revolution_count = total_write_count / size;
        const bool is_our_turn = revolution_count == slot_push_count;

        if (is_our_turn) 
        {
            /* 轮到我们去写对象 */
            if (m_write_count.compare_exchange_weak(total_write_count, total_write_count + 1U, std::memory_order_relaxed)) 
            {
                m_data[index].obj = element;
                m_data[index].push_count.store(slot_push_count + 1U, std::memory_order_release);
                return true;
            }
        } 
        else 
        {
            /* 我们去取下一个 */
            total_write_count = m_write_count.load(std::memory_order_relaxed);
        }
    }
}

template <typename T, size_t size> 
bool Queue<T, size>::Pop(T &element) 
{
    /* 原子的获得已经读取的对象数量 */
    size_t total_read_count = m_read_count.load(std::memory_order_relaxed);

    while (true) 
    {
        /* 此轮读取的值早数组中的下标 */
        const size_t will_read_index = total_read_count % size;
        const size_t pop_count = m_data[will_read_index].pop_count.load(std::memory_order_acquire);
        const size_t push_count = m_data[will_read_index].push_count.load(std::memory_order_relaxed);

        /* 说明此slot没有可读对象 */
        if (pop_count == push_count) 
            return false;

        /** 
         * 如果 已经循环的次数 == 该槽pop次数 为真，说明这个值被其他线程读完了。
         * 我们更新已读下标，当前调用改为去取下一个对象
         */
        const size_t revolution_count = total_read_count / size;
        const bool is_our_turn = revolution_count == pop_count;

        if (is_our_turn) 
        {
            /* 轮到我们去取对象 */
            if (m_read_count.compare_exchange_weak(total_read_count, total_read_count + 1U, std::memory_order_relaxed)) 
            {
                element = m_data[will_read_index].obj;
                m_data[will_read_index].pop_count.store(pop_count + 1U, std::memory_order_release);
                return true;
            }
        } 
        else 
            /* 取下一个对象 */
            total_read_count = m_read_count.load(std::memory_order_relaxed);
    }
}

}// namespace end