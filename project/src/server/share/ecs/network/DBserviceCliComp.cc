#include "share/ecs/network/DBServiceCliComp.hpp"
#include "share/ecs/network/Network.hpp"
#include "share/ecs/network/NetworkSystem.hpp"
#include "share/scene/SceneDefine.hpp"

namespace share::ecs::network
{

DBServiceCliComp::DBServiceCliComp(DBServiceCliCfg* cfg):
    engine::ecs::Component(EM_COMPONENT_TYPE_DBSERVICE_CLIENT),
    m_cfg(cfg)
{
}

DBServiceCliComp::~DBServiceCliComp()
{
}

void DBServiceCliComp::OnUpdate()
{
    auto connection = GetConnect();
    // 如果连接断开了，尝试建立连接
    if (connection == nullptr) {
        auto& db_service_cli_system = engine::ecs::GetSystem<DBServiceCliCompSystem>();
        db_service_cli_system->AsyncConnect(GetParentObject(), m_cfg->ip.c_str(), m_cfg->port, m_cfg->timeout);
        GAME_EXT1_LOG_INFO("try connect to database service!");
        return;
    }

    // 如果连接已经成立了，则检测是否发送heartbeat
    
}

bool DBServiceCliComp::IsConnected()
{
    if (m_connid <= 0) {
        return false;
    }

    auto db_connection = GetConnect();
    if (db_connection == nullptr)
        return false;
    
    return db_connection->IsClosed();
}

void DBServiceCliComp::Close()
{
    auto db_connection = GetConnect();
    if (db_connection == nullptr) {
        return;
    }

    db_connection->Close();
}

bool DBServiceCliComp::AsyncConnect(
    const char* ip,
    short port,
    int timeout)
{
    auto network_obj = share::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    if (network_obj == nullptr)
        return false;

    auto& network_sys_ref = engine::ecs::GetSystem<share::ecs::network::NetworkSystem>();
    network_sys_ref->AsyncConnect(network_obj, ip, port, timeout,
    [this, timeout](bbt::network::Errcode err, bbt::network::interface::INetConnectionSPtr conn){
        __OnConnect(err, conn, timeout);
    });
    return false;
}

void DBServiceCliComp::__OnConnect(
    bbt::network::Errcode err,
    bbt::network::interface::INetConnectionSPtr conn,
    int timeout)
{
    if (!err) {
        GAME_EXT1_LOG_ERROR(err.CWhat());
        return;
    }

    auto network_obj = share::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    std::shared_ptr<share::ecs::network::ConnMgr> connmgr_comp = nullptr;

    if (network_obj == nullptr) {
        GAME_EXT1_LOG_ERROR("can`t found Gameobject Tid=%d", (int)EM_ENTITY_TYPE_NETWORK);
        return;
    }

    auto comp = network_obj->GetComponent(EM_COMPONENT_TYPE_CONN_MGR);
    if ( comp == nullptr || (connmgr_comp = std::dynamic_pointer_cast<share::ecs::network::ConnMgr>(comp))) {
        GAME_EXT1_LOG_ERROR("can`t found Component Tid=%d", (int)EM_COMPONENT_TYPE_CONN_MGR);
        return;
    }

    auto dbconn = std::make_shared<share::ecs::network::DBServiceConnection>(
        connmgr_comp.get(), std::dynamic_pointer_cast<bbt::network::libevent::Connection>(conn), timeout);
    
    connmgr_comp->OnConnectAndAdd(dbconn);

}

std::shared_ptr<DBServiceConnection> DBServiceCliComp::GetConnect()
{
    auto network_obj = share::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    std::shared_ptr<share::ecs::network::ConnMgr> connmgr_comp = nullptr;

    auto comp = network_obj->GetComponent(EM_COMPONENT_TYPE_CONN_MGR);
    if ( comp == nullptr || (connmgr_comp = std::dynamic_pointer_cast<share::ecs::network::ConnMgr>(comp))) {
        GAME_EXT1_LOG_ERROR("can`t found Component Tid=%d", (int)EM_COMPONENT_TYPE_CONN_MGR);
        return;
    }

    auto connect = connmgr_comp->GetConnectById(m_connid);
    return std::static_pointer_cast<DBServiceConnection>(connect);
}


//////////////////////////////////////////////////////////////////////////
util::errcode::ErrOpt DBServiceCliCompSystem::Init(engine::ecs::GameObjectSPtr gameobj, DBServiceCliCfg* conn_cfg)
{
    if (gameobj == nullptr) {
        GAME_EXT1_LOG_ERROR("gameobj is null!");
        return util::errcode::ErrCode("gameobj is null!", util::errcode::ErrType::CommonErr, util::errcode::errenum::COMMON::PARAM_ERR);
    }

    auto dbservice_cli_comp = G_ComponentMgr()->Create<share::ecs::network::DBServiceCliComp>(conn_cfg);
    auto isok = gameobj->AddComponent(dbservice_cli_comp);
    Assert(isok);

    return std::nullopt;
}

std::shared_ptr<DBServiceCliComp> DBServiceCliCompSystem::GetComponent(engine::ecs::GameObjectSPtr gameobj)
{
    if (gameobj == nullptr)
        return nullptr;

    auto comp = gameobj->GetGameObject(EM_COMPONENT_TYPE_DBSERVICE_CLIENT);
    if (comp == nullptr)
        return nullptr;
    
    return std::dynamic_pointer_cast<DBServiceCliComp>(comp);
}

void DBServiceCliCompSystem::AsyncConnect(engine::ecs::GameObjectSPtr gameobj, const char* ip, short prot, int timeout)
{
    if (gameobj == nullptr) {

        return;
    }
}


}