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
    size_t w_count = m_write_count.load(std::memory_order_relaxed);

    while (true) {
        const size_t index = w_count % size;

        const size_t push_count =
            m_data[index].push_count.load(std::memory_order_acquire);
        const size_t pop_count =
            m_data[index].pop_count.load(std::memory_order_relaxed);

        if (push_count > pop_count) {
            return false;
        }

        const size_t revolution_count = w_count / size;
        const bool our_turn = revolution_count == push_count;

        if (our_turn) {
            /* Try to acquire the slot by bumping the monotonic write counter */
            if (m_write_count.compare_exchange_weak(w_count, w_count + 1U,
                                               std::memory_order_relaxed)) {
                m_data[index].val = element;
                m_data[index].push_count.store(push_count + 1U,
                                              std::memory_order_release);
                return true;
            }
        } else {
            w_count = m_write_count.load(std::memory_order_relaxed);
        }
    }
}

template <typename T, size_t size> 
bool Queue<T, size>::Pop(T &element) 
{
    size_t r_count = m_read_count.load(std::memory_order_relaxed);

    while (true) {
        const size_t index = r_count % size;

        const size_t pop_count =
            m_data[index].pop_count.load(std::memory_order_acquire);
        const size_t push_count =
            m_data[index].push_count.load(std::memory_order_relaxed);

        if (pop_count == push_count) {
            return false;
        }

        const size_t revolution_count = r_count / size;
        const bool our_turn = revolution_count == pop_count;

        if (our_turn) {
            /* Try to acquire the slot by bumping the monotonic read counter. */
            if (m_read_count.compare_exchange_weak(r_count, r_count + 1U,
                                               std::memory_order_relaxed)) {
                element = m_data[index].val;
                m_data[index].pop_count.store(pop_count + 1U,
                                             std::memory_order_release);
                return true;
            }
        } else {
            r_count = m_read_count.load(std::memory_order_relaxed);
        }
    }
}

template <typename T, size_t size>
std::optional<T> Queue<T, size>::PopOptional() 
{
    T element;
    bool result = Pop(element);

    if (result) {
        return element;
    } else {
        return {};
    }
}

}// namespace end