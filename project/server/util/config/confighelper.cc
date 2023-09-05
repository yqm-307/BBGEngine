#include <filesystem>
#include "util/config/confighelper.hpp"
#include "util/assert/Assert.hpp"

namespace game::util::config
{

ConfigHelper::ConfigHelper(const std::string& filename)
    :m_config_path(filename)
{
    auto isexist = std::filesystem::exists(filename);
    AssertWithInfo(isexist, "file not exist!");
    m_ini_reader = new util::ini::IniReader(filename);
    AssertWithInfo(m_ini_reader->ParseError() >= 0, "server config file error!");
}

ConfigHelper::~ConfigHelper()
{
    delete m_ini_reader;
}

}