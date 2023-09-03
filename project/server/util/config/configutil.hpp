#pragma once
#include <bbt/config/GlobalConfig.hpp>


namespace game::util::config
{

#define G_SetConfigPtr(type, value, enum) \
(BBT_CONFIG()->GetDynamicCfg()->SetEntry<type>(game::util::config::CfgNameMap[enum], value ))

#define G_GetConfigPtr(type, enum) \
(BBT_CONFIG()->GetDynamicCfg()->GetEntry<type>(game::util::config::CfgNameMap[enum]))

#define Entry(flag) {flag, #flag}

enum GameConfigType
{
    // bbt预留65535位, 我们从10w开始
    Cfg_Aoi         = 100001,   // aoi相关配置
    Cfg_evConnMgr   = 100002,   // 连接管理器相关配置
};

static std::unordered_map<GameConfigType, std::string> CfgNameMap = 
{
    Entry( Cfg_Aoi ),
    Entry( Cfg_evConnMgr ),
};


};