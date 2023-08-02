// #include "share/ecs/entity/aoi/Aoi.hpp"
#include "gameserver/init/LoadConfig.hpp"

// 数据库初始化
void DBInit()
{
}

// 网络初始化
void NetWorkInit()
{

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
}

int main()
{
    ConfigLoad();    
}