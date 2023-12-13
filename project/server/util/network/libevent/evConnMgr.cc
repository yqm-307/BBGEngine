#include "util/network/libevent/evConnMgr.hpp"
#include "util/config/Config.hpp"

namespace util::network::ev
{

inline ev::evConnectionSPtr SafeToEvConn(ConnectionSPtr raw_conn)
{
    auto conn = std::static_pointer_cast<evConnection>(raw_conn);
    DebugAssertWithInfo(conn != nullptr, "conversion failure!");
    return conn;
}

const std::unique_ptr<evConnMgr>& evConnMgr::GetInstance()
{
    static std::unique_ptr<evConnMgr> _instance{nullptr};
    if(_instance == nullptr)
        _instance = std::unique_ptr<evConnMgr>(new evConnMgr());
    
    return _instance;
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