#pragma once
#include "util/config/Config.hpp"
#include "util/ini/IniReader.hpp"
#include <string>

namespace util::config
{

class ConfigHelper
{
public:
    explicit ConfigHelper(const std::string& filename);
    virtual ~ConfigHelper();
protected:
    const std::string           m_config_path;
    util::ini::IniReader* m_ini_reader;
};

}