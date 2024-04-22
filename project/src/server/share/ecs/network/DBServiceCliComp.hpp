#pragma once
#include <bbt/base/clock/Clock.hpp>
#include <engine/ecs/component/Component.hpp>
#include <engine/ecs/gameobject/GameObject.hpp>
#include <engine/ecs/system/System.hpp>
#include <util/Define.hpp>
#include "share/ecs/network/DBServiceConnection.hpp"


namespace share::ecs::network
{

class DBServiceCliCompSystem;

struct DBServiceCliCfg
{
    std::string ip;
    short       port;
    int         timeout;
    int         connect_timeout;
    int         heartbeat;
};

typedef std::function<void(const bbt::network::Errcode&, std::shared_ptr<DBServiceConnection>)> OnDBServiceConnectionFunc;

class DBServiceCliComp:
    public engine::ecs::Component
{
    friend class DBServiceCliCompSystem;
    ComponentDeriveClassDef;
public:
    virtual ~DBServiceCliComp();

protected:
    DBServiceCliComp(DBServiceCliCfg cfg);
    virtual void OnUpdate() override;

    bool IsConnected();
    void Close();
    void DoHeartBeat(std::shared_ptr<DBServiceConnection> conn);
    std::shared_ptr<DBServiceConnection> GetConnect();
    bool AsyncConnect(
        const char* ip,
        short port,
        int timeout);

    void __OnConnect(bbt::network::Errcode err, bbt::network::interface::INetConnectionSPtr conn);
    
private:
    bbt::network::ConnId    m_connid{0};
    DBServiceCliCfg         m_cfg;

    //TODO 做一下连接超时
    bbt::clock::Timestamp<> m_last_heartbeat_timestamp;
    bool                    m_is_connecting{false};
};

class DBServiceCliCompSystem:
    public engine::ecs::System<DBServiceCliCompSystem>
{
public:
    util::errcode::ErrOpt   Init(engine::ecs::GameObjectSPtr gameobj, DBServiceCliCfg* conn_cfg);
    // void                    AsyncConnect(engine::ecs::GameObjectSPtr gameobj, const char* ip, short prot, int timeout);

private:
    std::shared_ptr<DBServiceCliComp> GetComponent(engine::ecs::GameObjectSPtr gameobj);
};
}