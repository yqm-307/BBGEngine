#include <bbt/network/Define.hpp>
#include "gameserver/init/LoadConfig.hpp"
#include "gameserver/scene/GameServerScene.hpp"
#include "util/random/GlobalRandom.hpp"
#include "share/scene/SceneDefine.hpp"

void RandomSeedInit()
{
    util::Random::SetSeed(bbt::timer::clock::now().time_since_epoch().count());
}

//TODO 实现pid文件
void PidFile()
{
    auto pid = ::getpid();
    GAME_BASE_LOG_INFO("process start! Pid=[ %d ]", pid);
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
    auto& cfg = server::init::ServerConfig::GetInstance();
    GAME_BASE_LOG_INFO("IP: %s  Port: %d", cfg->GetInstance()->GetServerIP().c_str(), cfg->GetInstance()->GetServerPort());

}

int main()
{
    // 加载全局配置
    ConfigLoad();
    // 设置随机数种子
    RandomSeedInit();
    // pid 文件
    PidFile();

    // 初始化游戏场景
    auto scene = new server::scene::GameServerScene();
    scene->Init();
    // 开启 World Scene
    scene->StartScene();
    delete scene;

}