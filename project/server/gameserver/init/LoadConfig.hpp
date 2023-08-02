#include "util/ini/IniReader.hpp"

namespace server::init
{

struct IniConfigList
{
    /* 服务器开放地址 */
    std::string server_ip;
    int         server_port;
};

/**
 * @brief 全局配置加载和配置检测规则
 */
class ServerConfig
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
    game::util::ini::IniReader           m_ini_reader;
    IniConfigList       m_config;
};

}