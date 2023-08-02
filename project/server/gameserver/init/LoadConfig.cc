#include "gameserver/init/LoadConfig.hpp"
#include "util/assert/Assert.hpp"

namespace server::init
{



std::string ServerConfig::m_server_inifile_path = "config/gameserver.ini";

ServerConfig::ServerConfig()
    :m_ini_reader(m_server_inifile_path)
{
    AssertWithInfo(m_ini_reader.ParseError() >= 0, "fatal: server config file error! please check server config is ready!");
    Init();
}

ServerConfig::~ServerConfig()
{
    
}

void ServerConfig::Init()
{
    m_config.server_ip = m_ini_reader.GetString("server", "ip", "");
    AssertWithInfo(m_config.server_ip != "", "fatal: server ip is null!");
    m_config.server_port = m_ini_reader.GetInteger("server", "port", 0);
    AssertWithInfo(m_config.server_port > 0, "fatal: server port is bad!");
}


ServerConfig* ServerConfig::GetInstance()
{
    ServerConfig* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new ServerConfig();
    }
    return instance;
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