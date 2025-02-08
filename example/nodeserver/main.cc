#include <bbt/network/Define.hpp>
#include "util/random/GlobalRandom.hpp"
#include "plugin/scene/SceneDefine.hpp"
#include "nodeserver/scene/NodeScene.hpp"
#include "nodeserver/config/LoadConfig.hpp"

/**
 * 加载全局配置
 */
void ConfigLoad()
{
    // !!!!!!!!!!! log debug 一定要放在最前面 !!!!!!!!!!!!!!
    int *debug_log = new int(1);
    BBT_CONFIG_QUICK_SET_DYNAMIC_ENTRY(int, debug_log, bbt::config::_BBTSysCfg::BBT_LOG_STDOUT_OPEN);
    
    // 服务器配置
    auto& cfg = BBTENGINE_NODE_NAMESPACE::config::ServerConfig::GetInstance();
    GAME_BASE_LOG_INFO("IP: %s  Port: %d", cfg->GetInstance()->GetServerIP().c_str(), cfg->GetInstance()->GetServerPort());

}

int main()
{
    ConfigLoad();

    // 初始化游戏场景
    auto scene = new BBTENGINE_NODE_NAMESPACE::scene::ServerScene();
    scene->Init();
    // 开启 World Scene
    scene->StartScene();
    delete scene;
}