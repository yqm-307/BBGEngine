#pragma once
#include <bbt/templateutil/Noncopyable.hpp>
#include <map>

namespace util::managerbase
{

template<typename _KeyType> class ManagerBase;

/**
 * @brief 被管理成员基类
 * 1、成员本身需要覆盖的实现 OnCreate 和 OnDestory，这两个函数会被 ManagerBase 在合适的时机调用;
 */
template<typename _KeyType>
class MemberBase
{
    friend class ManagerBase<_KeyType>;
public:

    typedef ManagerBase<_KeyType> ManagerType;
    typedef ManagerType::KeyType KeyType;

    virtual ~MemberBase() {m_mgr->OnMemberDestory();}
protected:
    virtual void OnCreate() = 0;
    virtual void OnDestory() = 0;

private:
    void OnInit(ManagerType* mgr) {m_mgr = mgr;}
    
    ManagerType*    m_mgr{nullptr};
    KeyType         m_key;
};


/**
 * @brief 标准的一个管理者模型需要实现的接口
 * 1、托管了被管理对象的构造
 */
template<typename _KeyType>
class ManagerBase:
    public bbt::templateutil::noncopyable
{
public:
    /// @brief 被管理对象的id，被纳入manager的对象需要用动态的对象id管理
    typedef _KeyType KeyType;
    /// @brief 被管理对象的类型
    typedef std::shared_ptr<MemberBase> MemberType;
    typedef std::pair<MemberType, bool>   Result;

protected:
    virtual Result      Search(KeyType key) = 0;
    virtual bool        IsExist(KeyType key) = 0;
    
    virtual bool        OnMemberCreate(std::shared_ptr<MemberBase> member) = 0;
    virtual bool        OnMemberDestory(KeyType member) = 0;

    virtual KeyType     GenerateKey(MemberType) = 0;

    template<typename GameObjectChildType, typename ...InitArgs>
    std::shared_ptr<GameObjectChildType> Create(InitArgs ...args)
    {
        /* 静态断言，要求模板提供类型只能是继承关系 */
        static_assert( std::is_base_of_v<MemberBase, GameObjectChildType> );

        auto sptr = std::shared_ptr<GameObjectChildType>(new GameObjectChildType(args...));
        DebugAssertWithInfo(sptr != nullptr, "managerbase create child type error!");

        sptr->OnInit(this);
        bool isok = OnMemberCreate(sptr);

        // XXX 如果GenerateID没有稳定了，可以去除debug断言
        DebugAssertWithInfo(isok, "managerbase OnMemberCreate() failed!");
        if(!isok) {
            sptr = nullptr; // 释放对象
            return nullptr;
        }

        return sptr;
    }

};


}// namespace util::managerbase

#include "./ManagerBaseTpl.hpp"