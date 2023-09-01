#include "util/network/libevent/evConnMgr.hpp"

namespace game::util::network::ev
{

evConnMgr* evConnMgr::GetInstance()
{
    static evConnMgr* instance = nullptr;
    if(instance == nullptr)
        instance = new evConnMgr();
    
    return instance;
}

evConnMgr::evConnMgr()
{
}

evConnMgr::~evConnMgr()
{
}


ConnectionSPtr evConnMgr::CreateConn(IOThread* thread, int newfd, Address peer_ip, Address local_ip)
{
    auto new_conn = std::make_shared<evConnection>(thread, newfd, peer_ip, local_ip);

    OnConnInit(new_conn);

    new_conn->SetOnDestory([this](evConnectionSPtr conn){
        OnConnDestory(conn);
    });

    return new_conn;
}

void evConnMgr::OnConnInit(evConnectionSPtr conn)
{
    DebugAssert(conn != nullptr);
    bool isok = false;
    {
        std::lock_guard<std::mutex> lock(m_conn_map_mutex);
        auto [it, exist] = m_conn_map.insert(std::make_pair(conn->GetSocket(), conn));
        isok = exist;
    }
    if(!isok)
        GAME_BASE_LOG_WARN("[evConnMgr::OnConnDestory] this is an unusual behavior, repeat init or no release prev conn, beacuse socket fd can be reused! sockfd=%d", conn->GetSocket());
    else
        GAME_EXT1_LOG_DEBUG("[evConnMgr::OnConnDestory] ConnMgr <== init connection! sockfd=%d", conn->GetSocket());
}

void evConnMgr::OnConnDestory(evConnectionSPtr conn)
{
    DebugAssert(conn != nullptr);
    bool isok = false;
    {
        std::lock_guard<std::mutex> lock(m_conn_map_mutex);
        auto it = m_conn_map.find(conn->GetSocket());
        if(it != m_conn_map.end())
        {
            isok = true;
            m_conn_map.erase(it);
        }
    }

    if(!isok)
        GAME_BASE_LOG_WARN("[evConnMgr::OnConnDestory] this is an unusual behavior, look out connection life cycle!");
    else
        GAME_EXT1_LOG_DEBUG("[evConnMgr::OnConnDestory] ConnMgr ==> release connection! sockfd=%d", conn->GetSocket());

}

ConnectionWKPtr evConnMgr::GetConnBySocket(int sockfd)
{
    return m_conn_map[sockfd];
}


}// namespace end