#pragma once
#include <vector>
#include <map>
#include <array>
#include <functional>

#define MYGAME_HASH_MAX_BUCKET 16

namespace game::util::hashmap
{

/**
 * @brief 简易实现 hashmap
 * 注意： 
 *  1、提供键的hash函数
 *  2、要求键是严格弱序的
 *  3、不支持重复键
 *  4、不支持引用类型（如果使用引用类型可能有问题）
 */
template<typename TKey, typename TValue, size_t BucketNum = 16>
class Hashmap
{
    typedef TKey    KeyType;
    typedef TValue  ValueType;
    typedef std::map<KeyType, ValueType>    HashBucket;
    typedef std::function<size_t(const KeyType&)>  HashFunction;
    typedef std::pair<ValueType, bool>      Result;
    typedef std::pair<HashBucket*, bool>    BucketResult;
public:
    /**
     * @brief 构造一个hashmap
     * 
     * @param bucket_size 桶数量
     * @param key_hash 键哈希函数
     */
    Hashmap(const HashFunction& key_hash, const ValueType& default_value);
    ~Hashmap();

    /* 根据key查找value */
    Result Find(const KeyType& key);
    /* 插入一个元素，并返回结果 */
    bool Insert(const KeyType& key, ValueType value);
    /* 删除一个元素，并返回 */
    Result Earse(const KeyType& key);
    /* 桶数量 */
    size_t BucketSize();
    /* Hashmap中元素数量 */
    size_t Size();
private:
    void InitHashmap();
    bool CheckIndex(size_t idx);
    BucketResult GetBucket(const KeyType& key);
private:
    size_t                  m_bucket_size{BucketNum};
    HashFunction            m_key_hash_func;
    std::vector<HashBucket> m_hash_map;
    const ValueType         m_default_value;    // 值为空时的默认值
};

}

#include "HashmapTpl.hpp"