#pragma once
#include "util/assert/Assert.hpp"
#include <bbt/templateutil/Noncopyable.hpp>
#include <map>

/* 标识类被 managerbase 管理 */
#define FlagManagedByManagerBase( _KeyType, _MemType) \
    friend util::managerbase::ManagerBase<_KeyType, _MemType>

namespace util::managerbase
{

template<typename _KeyType, typename _MemType> class ManagerBase;

/**
 * @brief 被管理成员基类
 * 1、成员本身需要覆盖的实现 OnCreate 和 OnDestory，这两个函数会被 ManagerBase 在合适的时机调用;
 */
template<typename _KeyType, typename _MemType>
class MemberBase
{
    friend class ManagerBase<_KeyType, _MemType>;
public:

    typedef ManagerBase<_KeyType, _MemType> ManagerType;

    virtual ~MemberBase() = 0;
    virtual _KeyType GetMemberId() const final;

private:
    void OnInit(ManagerType* mgr, _KeyType key);
    
    ManagerType*    m_mgr{nullptr};
    _KeyType        m_key;
};


/**
 * @brief 标准的一个管理者模型需要实现的接口
 * 1、托管了被管理对象的构造
 */
template<typename _KeyType, typename _MemType>
class ManagerBase:
    public bbt::templateutil::noncopyable
{
    friend class MemberBase<_KeyType, _MemType>;
public:
    /// @brief 被管理对象的id，被纳入manager的对象需要用动态的对象id管理
    typedef _KeyType KeyType;
    typedef std::shared_ptr<MemberBase<_KeyType, _MemType>> MemberBasePtr;
    /// @brief 被管理对象的类型
    typedef std::shared_ptr<_MemType> MemberPtr;
    typedef std::pair<MemberPtr, bool>   Result;

    template<typename MemberBaseChildType, typename ...InitArgs>
    std::shared_ptr<MemberBaseChildType> Create(InitArgs ...args);

protected:
    virtual Result      Search(KeyType key) = 0;
    virtual bool        IsExist(KeyType key) = 0;
    
    /**
     * @brief 当被管理者创建时调用此事件
     * 
     * @param member 
     * @return true 
     * @return false 
     */
    virtual bool OnMemberCreate(MemberPtr member) = 0;

    /**
     * @brief 当被管理者释放时调用此事件
     * 
     * @param member 
     * @return true 
     * @return false 
     */
    virtual bool OnMemberDestory(KeyType member) = 0;

    /**
     * @brief 生成id
     * 
     * @return KeyType 
     */
    virtual KeyType     GenerateKey(MemberPtr) = 0;

};


}// namespace util::managerbase

#include "./ManagerBaseTpl.hpp"