#pragma once
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
    short port;
    int timeout;
};

typedef std::function<void(const bbt::network::Errcode&, std::shared_ptr<DBServiceConnection>)> OnDBServiceConnectionFunc;

class DBServiceCliComp:
    public engine::ecs::Component
{
    friend class DBServiceCliCompSystem;
public:
    virtual ~DBServiceCliComp();

protected:
    DBServiceCliComp(DBServiceCliCfg* cfg);
    virtual void OnUpdate() override;

    bool IsConnected();
    void Close();
    bool AsyncConnect(
        const char* ip,
        short port,
        int timeout);
    void __OnConnect(bbt::network::Errcode err, bbt::network::interface::INetConnectionSPtr conn, int timeout);
    std::shared_ptr<DBServiceConnection> GetConnect();
    
private:
    bbt::network::ConnId    m_connid{0};
    DBServiceCliCfg*        m_cfg{nullptr};

    //TODO 做一下连接超时
    // int pre_try_conn_timestamp;
};

class DBServiceCliCompSystem:
    public engine::ecs::System<DBServiceCliComp>
{
public:
    util::errcode::ErrOpt   Init(engine::ecs::GameObjectSPtr gameobj, DBServiceCliCfg* conn_cfg);
    void                    AsyncConnect(engine::ecs::GameObjectSPtr gameobj, const char* ip, short prot, int timeout);

private:
    std::shared_ptr<DBServiceCliComp> GetComponent(engine::ecs::GameObjectSPtr gameobj);
};
}