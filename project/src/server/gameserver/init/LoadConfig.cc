#include "gameserver/init/LoadConfig.hpp"
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"

namespace server::init
{



std::string ServerConfig::m_server_inifile_path = "config/gameserver.ini";

ServerConfig::ServerConfig()
    :ConfigHelper(m_server_inifile_path)
{
    Init();
    CheckConfig();
}

ServerConfig::~ServerConfig()
{
}

void ServerConfig::Init()
{
    m_config.server_ip = m_ini_reader->GetString("server", "ip", "");
    AssertWithInfo(m_config.server_ip != "", "fatal: server ip is null!");
    m_config.server_port = m_ini_reader->GetInteger("server", "port", 0);
    AssertWithInfo(m_config.server_port > 0, "fatal: server port is bad!");

    m_config.m_connmgr_cfg.m_heart_beat_once_ms = m_ini_reader->GetInteger("heartbeat", "interval", 0);
    m_config.m_connmgr_cfg.m_heart_beat_timeout_ms = m_ini_reader->GetInteger("heartbeat", "checkonce", 0);
    G_SetConfigPtr(util::config::evConnMgrConfig, &m_config.m_connmgr_cfg, util::config::Cfg_evConnMgr);
}

void ServerConfig::CheckConfig()
{
    for(auto oncheck_callback : m_oncheck_queue)
    {
        auto [isok, msg] = oncheck_callback();
        if(!isok){
            GAME_BASE_LOG_ERROR("[ServerConfig::CheckConfig] OnCheckCallback ErrMsg: %s", msg.c_str());
        }
        DebugAssert(isok);  // 配置检测错误
    }
}

std::pair<bool,std::string> ServerConfig::CheckHeartbeat()
{
    auto timeout = m_config.m_connmgr_cfg.m_heart_beat_timeout_ms;
    auto check_ms = m_config.m_connmgr_cfg.m_heart_beat_once_ms;
    if(timeout <= 0 || check_ms <= 0){
        std::string str;
        return {false, bbt::log::format("fatal: check_ms is invalid! timeout=%d check_ms=%d", timeout, check_ms)};
    }
    /**
     * 关于心跳检测时间
     * 
     * 关于这两个时间，首先服务器心跳是一个定时器定时去遍历所有的连接。
     * 那么这个服务器检测时间需要可以检测出来连接断开。
     * 
     * ps：当前的设定对于心跳是弱检测的，其实有一定的精度损失
     * 
     * 首先不能大于心跳超时的两倍，否则检测过于频繁，会造成没必要的检测。
     * 其次不能少于心跳超时事件，否则会导致心跳超时很久还没有检测。
     * 
     * 最终结论：检测时间 * 2 > 心跳超时时间 > 检测时间
     */
    bool cfg_is_err = !( check_ms < timeout && timeout < (check_ms * 2) );
    if(cfg_is_err){
        return {false, bbt::log::format("please check config about of evConnMgr! timeout=%d check_ms=%d", timeout, check_ms)};
    }

    return {true, "nothing"};
}


const std::unique_ptr<ServerConfig>& ServerConfig::GetInstance()
{
    static std::unique_ptr<ServerConfig> _instance{nullptr};
    if(_instance == nullptr)
    {
        _instance = std::unique_ptr<ServerConfig>(new ServerConfig());
    }
    return _instance;
}

std::string ServerConfig::GetServerIP()
{
    return m_config.server_ip;
}

int ServerConfig::GetServerPort()
{
    return m_config.server_port;
}


}