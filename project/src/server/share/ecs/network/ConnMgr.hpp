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
    ComponentDeriveClassDef;
public:
    ~ConnMgr();
    bool Init();
    
    bool DelConnect(bbt::network::ConnId conn);
    bool AddConnect(std::shared_ptr<Connection> conn);
    void OnTimeout(Connection* conn);
protected:
    ConnMgr(const ServerCfg& cfg);
    virtual void OnUpdate() override {};

    /* network回调，当建立新连接时触发 */
    void OnAcceptAndInitConn(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn);
    /* 建立了连接后，调用此函数注册 */
    void OnConnectAndAdd(std::shared_ptr<Connection> new_conn);
private:
    std::unordered_map<engine::ecs::ComponentId, std::shared_ptr<Connection>>    m_conn_map;
    ServerCfg   m_cfg;
};

}