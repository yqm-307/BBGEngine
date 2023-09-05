#pragma once
// #include "util/ini/IniReader.hpp"
#include "util/config/confighelper.hpp"

namespace server::init
{

struct IniConfigList
{
    /* 服务器开放地址 */
    std::string server_ip;
    int         server_port;
    game::util::config::evConnMgrConfig
                m_connmgr_cfg;
    
};

/**
 * @brief 全局配置加载和配置检测规则
 */
class ServerConfig:
    public game::util::config::ConfigHelper
{
public:
    static ServerConfig* GetInstance();

    std::string GetServerIP();
    int         GetServerPort();
private:
    ServerConfig();
    ~ServerConfig();

    void Init();

private:
    static std::string  m_server_inifile_path;
    IniConfigList       m_config;
};

}