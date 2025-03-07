#pragma once
#include "util/hashmap/Hashmap.hpp" // 为了语法提示
#include "util/assert/Assert.hpp"
#include "util/other/Math.hpp"
#include <bbt/core/Define.hpp>

// using namespace
namespace util::hashmap
{

template<typename TKey, typename TValue, size_t BucketNum>
Hashmap<TKey, TValue, BucketNum>::Hashmap(const HashFunction& key_hash, const ValueType&& default_value)
    :m_key_hash_func(key_hash),
    m_default_value(default_value),
    m_hash_map(m_bucket_size)

{
    AssertWithInfo(key_hash != nullptr, "hash function not null!");
    AssertWithInfo(m_bucket_size > 0 && m_bucket_size <= MYGAME_HASH_MAX_BUCKET, "bucket num too much!"); /* 桶数量限制 */
    InitHashmap();
}

template<typename TKey, typename TValue, size_t BucketNum>
Hashmap<TKey, TValue, BucketNum>::Hashmap(const HashFunction& key_hash, const ValueType& default_value, std::initializer_list<std::pair<KeyType, ValueType>> args)
    :m_key_hash_func(key_hash),
    m_default_value(default_value),
    m_hash_map(m_bucket_size)
{
    AssertWithInfo(key_hash != nullptr, "hash function not null!");
    AssertWithInfo(m_bucket_size > 0 && m_bucket_size <= MYGAME_HASH_MAX_BUCKET, "bucket num too much!"); /* 桶数量限制 */
    InitHashmap();
    int args_item_num = args.size();
    int success_insert_num = Insert(args);
    AssertWithInfo(args_item_num == success_insert_num, "Hashmap init error!");
}

template<typename TKey, typename TValue, size_t BucketNum>
Hashmap<TKey, TValue, BucketNum>::~Hashmap()
{
    int size = Size();
    DebugAssertWithInfo(Clear() == size, "error");
}

template<typename TKey, typename TValue, size_t BucketNum>
void Hashmap<TKey, TValue, BucketNum>::InitHashmap() const
{
}

#pragma region "增删改查接口实现"

template<typename TKey, typename TValue, size_t BucketNum>
typename Hashmap<TKey, TValue, BucketNum>::Result Hashmap<TKey, TValue, BucketNum>::Find(const KeyType& key) const
{
    size_t bucket_index = m_key_hash_func(key);
    if( bbt_unlikely(!CheckIndex(bucket_index)) )
        return {m_default_value, false};

    auto& bucket = m_hash_map[bucket_index];
    auto it = bucket.find(key);
    if( bbt_unlikely(it == bucket.end()))
        return {m_default_value, false};
        
    return {it->second, true};
}

template<typename TKey, typename TValue, size_t BucketNum>
bool Hashmap<TKey, TValue, BucketNum>::Insert(const KeyType& key, ValueType value)
{
    auto [bucket, isok_get_bucket] = GetBucket(key);
    if( bbt_unlikely(!isok_get_bucket) )
        return false;
    auto [it, isok_insert_into_bucket] = bucket->insert(std::make_pair(key, value));
    return isok_insert_into_bucket;
}

template<typename TKey, typename TValue, size_t BucketNum>
size_t Hashmap<TKey, TValue, BucketNum>::Insert(std::initializer_list<KVPair> args)
{
    size_t insert_success_num = 0;
    for(auto arg: args)
    {
        if(Insert(arg.first, arg.second))
            insert_success_num++;
    }
    return insert_success_num;
}


template<typename TKey, typename TValue, size_t BucketNum>
typename Hashmap<TKey, TValue, BucketNum>::Result Hashmap<TKey, TValue, BucketNum>::Earse(const KeyType& key)
{
    auto [bucket, isok] = GetBucket(key);
    if( bbt_unlikely(!isok) )
        return {m_default_value, false};
        
    auto it = bucket->find(key);
    if( bbt_unlikely(it == bucket->end()) )
        return {m_default_value, false};
        
    bucket->erase(it);
    return {it->second, true};
}

template<typename TKey, typename TValue, size_t BucketNum>
size_t Hashmap<TKey, TValue, BucketNum>::Clear()
{
    size_t clear_item_num = 0;
    for(auto && item : m_hash_map)
    {
        clear_item_num += item.size();
        item.clear();
    }
    return clear_item_num;
}


#pragma endregion









template<typename TKey, typename TValue, size_t BucketNum>
bool Hashmap<TKey, TValue, BucketNum>::CheckIndex(size_t idx) const
{
    return ( idx >= 0 && idx < m_bucket_size );
}

template<typename TKey, typename TValue, size_t BucketNum>
typename Hashmap<TKey, TValue, BucketNum>::BucketResult Hashmap<TKey, TValue, BucketNum>::GetBucket(const KeyType& key)
{
    auto bucket_index = m_key_hash_func(key);
    if( bbt_unlikely(!CheckIndex(bucket_index)) )
        return {nullptr, false};

    return { &(m_hash_map[bucket_index]), true};
}

template<typename TKey, typename TValue, size_t BucketNum>
size_t Hashmap<TKey, TValue, BucketNum>::BucketSize() const
{
    return m_bucket_size;
}

template<typename TKey, typename TValue, size_t BucketNum>
size_t Hashmap<TKey, TValue, BucketNum>::Size() const
{
    size_t count = 0;
    for(auto&& map : m_hash_map)
    {
        count += map.size();
    }
    return count;
}

template<typename TKey, typename TValue, size_t BucketNum>
void Hashmap<TKey, TValue, BucketNum>::Foreach_Random(const ForeachFunction& func) const
{
    std::vector<int> idxs(m_bucket_size);
    int i = 0;
    std::for_each_n(idxs.begin(), m_bucket_size, [&](int& val){ val = i++; });
    [[maybe_unused]] bool isok = other::Math::Derange(idxs);
    DebugAssertWithInfo(isok, "game::other::Math::Derange error!");

    for(auto&& it_bucket : idxs)
    {
        for(auto&& item : m_hash_map[it_bucket])
        {
            func(item->second);
        }
    }
}


}