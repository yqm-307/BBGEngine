#include "gameserver/init/LoadConfig.hpp"
#include "gameserver/network/Network.hpp"
#include "gameserver/scene/GameServerScene.hpp"
#include "util/random/GlobalRandom.hpp"


namespace g_global
{
static server::network::Network* network_scene;
};

void RandomSeedInit()
{
    game::util::Random::SetSeed(bbt::timer::clock::now().time_since_epoch().count());
}

// 数据库初始化
void DBInit()
{
}

// 网络初始化
void NetWorkInit()
{
    g_global::network_scene = new  server::network::Network("127.0.0.1", 10010);
    g_global::network_scene->SyncStart();
}

// 游戏场景初始化
server::scene::GameServerScene* GameSceneInit()
{
    auto ptr = new server::scene::GameServerScene();
    return ptr;
}

/**
 * 加载全局配置
 */
void ConfigLoad()
{
    // !!!!!!!!!!! log debug 一定要放在最前面 !!!!!!!!!!!!!!
    int *debug_log = new int(1);
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, debug_log, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
    
    // 服务器配置
    auto cfg = server::init::ServerConfig::GetInstance();
    GAME_BASE_LOG_INFO("IP: %s  Port: %d", cfg->GetInstance()->GetServerIP().c_str(), cfg->GetInstance()->GetServerPort());


    auto aoi_cfg = new game::util::config::AoiConfig();
    aoi_cfg->m_map_x = 100; aoi_cfg->m_map_y = 100; aoi_cfg->m_map_z = 1;
    aoi_cfg->m_tower_x = 3; aoi_cfg->m_tower_y = 3; aoi_cfg->m_tower_z = 1;
    
    G_SetConfigPtr(game::util::config::AoiConfig, aoi_cfg, game::util::config::GameConfigType::Cfg_Aoi);
}

int main()
{
    ConfigLoad();
    RandomSeedInit();
    NetWorkInit();
    auto scene = GameSceneInit();

    scene->StartScene();
    delete scene;
}