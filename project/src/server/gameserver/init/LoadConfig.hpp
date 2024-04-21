#pragma once
// #include "util/ini/IniReader.hpp"
#include "util/config/ConfigHelper.hpp"
#include <functional>
#include <set>
#include <unordered_set>

namespace server::init
{

struct DBServiceConfig
{
    std::string dbservice_ip;
    int dbservice_port;
    int connect_timeout_ms;
};

struct IniConfigList
{
    /* 服务器开放地址 */
    std::string server_ip;
    int         server_port;
    util::config::evConnMgrConfig
                m_connmgr_cfg;
    DBServiceConfig m_db_service_cfg;
};

/**
 * @brief 全局配置加载和配置检测规则
 */
class ServerConfig:
    public util::config::ConfigHelper
{
    typedef std::function<std::pair<bool, std::string>()>   OnCheckCallback;
public:
    static const std::unique_ptr<ServerConfig>& GetInstance();
    ~ServerConfig();

    const std::string& GetServerIP() const;
    int         GetServerPort() const;

    const std::string& GetDBServiceIP() const;
    int         GetDBServicePort() const;
    int         GetDBServiceConnTimeout() const;
private:
    ServerConfig();

    void Init();
    void CheckConfig();
private:
    /*----------- 配置检测函数，加载服务器配置的时候检测 -----------*/
    std::vector<OnCheckCallback> m_oncheck_queue{
        [=](){return ServerConfig::CheckHeartbeat();},  // heartbeat checker
    };
    std::pair<bool,std::string> CheckHeartbeat();
private:
    static std::string  m_server_inifile_path;
    IniConfigList       m_config;

    
};

}