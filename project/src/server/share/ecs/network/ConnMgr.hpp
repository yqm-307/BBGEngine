#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/network/Connection.hpp"

namespace share::ecs::network
{

struct ServerCfg
{
    // 服务器ip
    std::string ip;
    // 服务器监听端口
    short       port;
    int         connent_timeout;
};

class ConnMgr:
    public engine::ecs::Component
{
public:
    ~ConnMgr();
    bool Init();
    
protected:
    ConnMgr(const ServerCfg& cfg);

    /* network回调，当建立新连接时触发 */
    void OnAcceptAndInitConn(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn);
private:
    std::unordered_map<engine::ecs::ComponentId, std::shared_ptr<Connection>>    m_conn_map;
    ServerCfg   m_cfg;
};

}