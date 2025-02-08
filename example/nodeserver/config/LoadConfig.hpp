#pragma once
#include <functional>
#include <set>
#include <unordered_set>
// #include "util/ini/IniReader.hpp"
#include "util/config/ConfigHelper.hpp"
#include "nodeserver/NodeDefine.hpp"

namespace BBTENGINE_NODE_NAMESPACE::config
{

struct DBServiceConfig
{
    std::string             ip;                 // 服务ip
    short                   port;               // 服务端口
    int                     timeout;            // tcp连接空闲超时
    int                     connect_timeout;    // 连接对端超时
    int                     heartbeat;          // heartbeat 发送间隔
};

struct IniConfigList
{
    /* 服务器开放地址 */
    std::string                         server_ip;
    int                                 server_port;
    util::config::evConnMgrConfig       m_connmgr_cfg;
    DBServiceConfig                     m_db_service_cfg;
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

    const DBServiceConfig *const GetDBServiceCfg() const;
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