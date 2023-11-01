#include "util/network/ConnMgr.hpp"
#include "util/log/Log.hpp"


namespace util::network
{

ConnMgr::ConnMgr()
{
}

ConnMgr::~ConnMgr()
{
}

ConnMgr::Result ConnMgr::Search(KeyType key)
{
    auto it = m_conn_map.find(key);
    if(it == m_conn_map.end())
        return {nullptr, false};

    auto ptr = (it->second).lock();
    if(ptr == nullptr)
        BBT_BASE_LOG_WARN("[ConnMgr::Search] connect is not exist! key=%d", key);

    return {ptr, true};
}

bool ConnMgr::IsExist(KeyType key)
{
    auto it = m_conn_map.find(key);
    return !(it == m_conn_map.end());
}

bool ConnMgr::OnMemberCreate(MemberPtr member_ptr)
{
    if(member_ptr == nullptr)
        BBT_BASE_LOG_WARN("[ConnMgr::OnMemberCreate] connect can`t convert to derive class!");

    bool isok = false;
    {
        std::lock_guard<std::mutex> lock(m_conn_map_mutex);
        auto [it, existed] = m_conn_map.insert(std::make_pair(member_ptr->GetMemberId(), member_ptr));
        isok = existed;
    }
    if(!isok)
        GAME_BASE_LOG_DEBUG("[ConnMgr::OnMemberCreate] this is an unusual behavior, repeat init or no release prev conn! sockfd=%d", member_ptr->GetSocket());
    else
        GAME_EXT1_LOG_DEBUG("[ConnMgr::OnMemberCreate] ConnMgr <== init connection! sockfd=%d", member_ptr->GetSocket());
    return isok;
}

bool ConnMgr::OnMemberDestory(KeyType connid)
{
    DebugAssert(connid >= 0);
    bool isok = false;
    ConnectionSPtr conn_ptr = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_conn_map_mutex);
        auto it = m_conn_map.find(connid);
        if(it != m_conn_map.end())
        {
            isok = true;
            m_conn_map.erase(it);
        }

        conn_ptr = (it->second).lock();
        if(!isok)
            GAME_BASE_LOG_WARN("[ConnMgr::OnMemberDestory] this is an unuseal behavior, look out connection life cycle!");
    }

    return isok;
}

ConnMgr::KeyType ConnMgr::GenerateKey(MemberPtr)
{
    return GenerateConnectID();
}

}// namespace end