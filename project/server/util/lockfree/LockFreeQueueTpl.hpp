#include "util/lockfree/LockFreeQueue.hpp"
namespace game::util::lockfree
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
    /* 已经写了多少个了 */
    size_t current_count = m_write_count.load(std::memory_order_relaxed);

    while (true) {
        /* 计算循环数组的下标 */
        const size_t index = current_count % size;
        /* 获取该槽的push计数，后面对这个值还有修改，需要保证不会吧重排 */
        const size_t slot_push_count = m_data[index].push_count.load(std::memory_order_acquire);
        /* 获取该槽的pop计数，在push操作中，只读所以保证原子性即可 */
        const size_t slot_pop_count = m_data[index].pop_count.load(std::memory_order_relaxed);

        /* 首次确认当前槽的插入合法性，对于每个槽，push操作一定小于等于pop操作次数 */
        if (slot_push_count > slot_pop_count) 
            return false;

        /* 已经循环圈数 */ 
        const size_t revolution_count = current_count / size;
        /* 再次确认这个槽的插入合法性 */ 
        const bool our_turn = revolution_count == slot_push_count;

        if (our_turn) 
        {
            /* 通过竞争获得写入的权限，保证原子性即可 */
            if (m_write_count.compare_exchange_weak(current_count, current_count + 1U, std::memory_order_relaxed)) 
            {
                /* 内部是安全的操作，因为获得了第 m_write_count 位的写入权限，其他人需要竞争后面的位置 */
                m_data[index].val = element;
                /* 使用 memory_order_release 是因为，作为最终的标志结束的标志位。不允许重排到前面 */
                m_data[index].push_count.store(slot_push_count + 1U, std::memory_order_release);
                return true;
            }
        } 
        else 
        {
            current_count = m_write_count.load(std::memory_order_relaxed);
        }
    }
}

template <typename T, size_t size> 
bool Queue<T, size>::Pop(T &element) 
{
    /* 读保证原子性即可 */
    size_t r_count = m_read_count.load(std::memory_order_relaxed);

    while (true) 
    {
        const size_t index = r_count % size;
        /* 有修改，保证可见性，防止重排 */
        const size_t pop_count = m_data[index].pop_count.load(std::memory_order_acquire);
        const size_t push_count = m_data[index].push_count.load(std::memory_order_relaxed);

        if (pop_count == push_count) 
            return false;

        const size_t revolution_count = r_count / size;
        const bool our_turn = revolution_count == pop_count;

        if (our_turn) 
        {
            /* 通过竞争获得读取的权限，保证原子性即可 */
            if (m_read_count.compare_exchange_weak(r_count, r_count + 1U, std::memory_order_relaxed)) 
            {
                element = m_data[index].val;
                m_data[index].pop_count.store(pop_count + 1U, std::memory_order_release);
                return true;
            }
        } 
        else 
            r_count = m_read_count.load(std::memory_order_relaxed);
    }
}

}// namespace end