#pragma once
#include "util/config/config.hpp"
#include "util/ini/IniReader.hpp"
#include <string>

namespace game::util::config
{

class ConfigHelper
{
public:
    explicit ConfigHelper(const std::string& filename);
    virtual ~ConfigHelper();
protected:
    const std::string           m_config_path;
    game::util::ini::IniReader* m_ini_reader;
};

}