#pragma once
#include <bbt/templateutil/Noncopyable.hpp>
#include <map>

namespace util::managerbase
{


/**
 * @brief 标准的一个管理者模型需要实现的接口
 * 
 */
template<typename _KeyType, class _ObjType>
class ManagerBase:
    public bbt::templateutil::noncopyable
{
public:
    typedef _KeyType KeyType;
    typedef _ObjType ValueType;
    typedef std::pair<_ObjType, bool>   Result;
protected:
    virtual Result      Search(KeyType key) = 0;
    virtual bool        IsExist(KeyType key) = 0;
};

}