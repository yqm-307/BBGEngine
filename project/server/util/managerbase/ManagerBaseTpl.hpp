#pragma once
#include "./ManagerBase.hpp"


namespace util::managerbase
{

template<typename _KeyType, typename _MemType>
_KeyType MemberBase<_KeyType, _MemType>::GetMemberId() const
{
    return m_key;
}

template<typename _KeyType, typename _MemType>
void MemberBase<_KeyType, _MemType>::OnInit(ManagerType* mgr, _KeyType key) 
{
    m_mgr = mgr;
    m_key = key;
}

template<typename _KeyType, typename _MemType>
template<typename MemberBaseChildType, typename ...InitArgs>
std::shared_ptr<MemberBaseChildType> ManagerBase<_KeyType, _MemType>::Create(InitArgs ...args)
{
    /* 静态断言，要求模板提供类型只能是继承关系 */
    static_assert( std::is_base_of_v<MemberBase<_KeyType, _MemType>, MemberBaseChildType> );

    auto sptr = std::shared_ptr<MemberBaseChildType>(new MemberBaseChildType(args...));
    DebugAssertWithInfo(sptr != nullptr, "managerbase create child type error!");

    sptr->OnInit(this, GenerateKey(sptr));
    bool isok = OnMemberCreate(sptr);

    // XXX 如果GenerateID没有稳定了，可以去除debug断言
    DebugAssertWithInfo(isok, "managerbase OnMemberCreate() failed!");
    if(!isok) {
        sptr = nullptr; // 释放对象
        return nullptr;
    }

    return sptr;
}

}// namespace util::managerbase