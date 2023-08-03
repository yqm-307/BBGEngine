#pragma once
#include <atomic>
#include <array>
#include <bbt/templateutil/Comparator.hpp>
#include <bbt/timer/Clock.hpp>

namespace game::util::lockfree
{


class TmpVersion
    :public bbt::templateutil::comparator<uint64_t>
{
    typedef bbt::templateutil::comparator<uint64_t> CompItem;
public:
    TmpVersion()
    {
        this->SetValue(bbt::timer::clock::now<bbt::timer::clock::ns>().time_since_epoch().count());
    }
    virtual bool operator==(const CompItem& rval) const override
    {
        return ((it_) == (rval.GetValue()));
    }
    virtual bool operator>(const CompItem& rval) const override
    {
        return ((it_) > (rval.GetValue()));
    }
};

/**
 * @brief 节点
 * 
 * @tparam T 保存元素类型
 * @tparam VersionType 要求VersionType类在构造时自动生成一个不重复的id，支持比较操作
 */
template<typename T, typename VersionType>
struct LockFreeQueueSlot
{
    T m_item;
    VersionType m_version;
};


template<typename T, size_t Num>
class LockFreeQueue
{
public:
    explicit LockFreeQueue();
    ~LockFreeQueue();

    T Pop();
    void Push();
    size_t Size();
    bool Empty();
    void Clear();
private:
    std::array<LockFreeQueueSlot<T, TmpVersion>, Num> m_arr;
};


}