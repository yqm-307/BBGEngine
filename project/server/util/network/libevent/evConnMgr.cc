#include "util/network/libevent/evConnMgr.hpp"
#include "util/config/config.hpp"

namespace game::util::network::ev
{

inline ev::evConnectionSPtr SafeToEvConn(ConnectionSPtr raw_conn)
{
    auto conn = std::static_pointer_cast<evConnection>(raw_conn);
    DebugAssertWithInfo(conn != nullptr, "conversion failure!");
    return conn;
}

evConnMgr* evConnMgr::GetInstance()
{
    static evConnMgr* instance = nullptr;
    if(instance == nullptr)
        instance = new evConnMgr();
    
    return instance;
}

evConnMgr::evConnMgr()
{
    InitCfg();
}

evConnMgr::~evConnMgr()
{
}



void evConnMgr::InitCfg()
{
    auto* cfg_ptr = G_GetConfigPtr(config::evConnMgrConfig, config::Cfg_evConnMgr); 
    m_check_heartbeat_interval_ms = cfg_ptr->m_heart_beat_once_ms;
    m_heartbeat_timeout_duration = bbt::timer::clock::ms(cfg_ptr->m_heart_beat_timeout_ms);
}

#pragma region "事件控制"

void evConnMgr::InitEvent()
{
    DebugAssertWithInfo(m_thread != nullptr, "thread no init or init timing error!");// io thread 没有初始化，或者初始化时机错误
    
}

void evConnMgr::InitHeartBeatEvent()
{
    // m_heartbeat_check_event = event_new();
    Assert(false);
}

void evConnMgr::DestoryHeartBeatEvent()
{
}

#pragma endregion
void evConnMgr::SetIOThread(IOThread* thread)
{
    DebugAssert(thread != nullptr);
    if(m_thread == nullptr)
        m_thread = thread;
    else
        GAME_BASE_LOG_WARN("[evConnMgr::SetIOThread] repeat init io thread!");
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

void evConnMgr::OnHeartBeatOnce()
{
    {
        std::lock_guard<std::mutex> lock(m_conn_map_mutex);
        for(auto&& conn : m_conn_map)
        {
            auto ev_conn = SafeToEvConn(conn.second);
            if(ev_conn)
            {
                auto time = ev_conn->GetPrevHeartBeatTimestamp() + m_heartbeat_timeout_duration;
                auto time_now = bbt::timer::clock::now();
                if( time_now > time )
                {
                    // 超时了
                    ev_conn->Close();
                }
            }
        }
    }
}


}// namespace end