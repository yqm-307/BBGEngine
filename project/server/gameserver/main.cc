#include "gameserver/init/LoadConfig.hpp"
#include "gameserver/network/Network.hpp"


namespace g_global
{
static server::network::Network* network_scene;
};

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
void GameSceneInit()
{

}

/**
 * 加载全局配置
 */
void ConfigLoad()
{
    auto cfg = server::init::ServerConfig::GetInstance();
    printf("%s\n", cfg->GetInstance()->GetServerIP().c_str());
    printf("%d\n", cfg->GetInstance()->GetServerPort());
    int *debug_log = new int(1);
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, debug_log, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
}

int main()
{
    ConfigLoad();    
    NetWorkInit();
    for(;;){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };
}