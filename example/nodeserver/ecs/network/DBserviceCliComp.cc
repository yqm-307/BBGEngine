#include "plugin/ecs/network/NetworkSystem.hpp"
#include "plugin/scene/SceneDefine.hpp"
#include "nodeserver/ecs/network/DBServiceCliComp.hpp"
#include "nodeserver/session/DBServiceSession.hpp"

namespace plugin::ecs::network
{

DBServiceCliComp::DBServiceCliComp(DBServiceCliCfg cfg):
    engine::ecs::Component(EM_COMPONENT_TYPE_DBSERVICE_CLIENT),
    m_cfg(cfg)
{
    m_last_heartbeat_timestamp = bbt::clock::now<>();
    m_last_try_connect_timestamp = bbt::clock::now<>() - bbt::clock::ms(m_cfg.connect_timeout);
}

DBServiceCliComp::~DBServiceCliComp()
{
}

void DBServiceCliComp::OnUpdate()
{
    auto connection = GetConnect();
    // 如果连接断开了，尝试建立连接
    if (connection == nullptr && !m_is_connecting && bbt::clock::is_expired<bbt::clock::ms>(m_last_try_connect_timestamp + bbt::clock::ms(m_cfg.connect_timeout + 1000))) {
        AsyncConnect(m_cfg.ip.c_str(), m_cfg.port, m_cfg.connect_timeout);
        m_is_connecting = true;
        m_last_try_connect_timestamp = bbt::clock::now<>();
        GAME_EXT1_LOG_INFO("try connect to database service!");
        return;
    }

    // 如果连接已经成立了，则检测是否发送heartbeat
    if (connection != nullptr && !m_is_connecting && bbt::clock::is_expired<bbt::clock::ms>(m_last_heartbeat_timestamp + bbt::clock::ms(m_cfg.heartbeat)))
    {
        DoHeartBeat(connection);        
    }
}

void DBServiceCliComp::DoHeartBeat(std::shared_ptr<DBServiceConnection> conn)
{
    DB_HEART_BEAT_REQ   req;
    bbt::buffer::Buffer output_buffer;
    std::string         protocol;
    uint32_t            protolen = 0;
    uint32_t            protoid = plugin::session::DBServiceProtoId::EM_DB_SERVICE_PROTO_HEARTBEAT_REQ;

    m_last_heartbeat_timestamp = bbt::clock::now<>();
    req.set_timestamp(m_last_heartbeat_timestamp.time_since_epoch().count());
    protocol = req.SerializeAsString();

    protolen = protocol.size() + sizeof(protolen) + sizeof(protoid);
    output_buffer.WriteInt32(protolen);
    output_buffer.WriteInt32(protoid);
    output_buffer.WriteString(protocol);

    conn->Send(output_buffer.Peek(), output_buffer.DataSize());
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
    auto network_obj = plugin::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    if (network_obj == nullptr)
        return false;

    auto& network_sys_ref = engine::ecs::GetSystem<plugin::ecs::network::NetworkSystem>();
    network_sys_ref->AsyncConnect(network_obj, ip, port, timeout,
    [this, timeout](bbt::network::Errcode err, bbt::network::interface::INetConnectionSPtr conn){
        __OnConnect(err, conn);
    });
    return false;
}

void DBServiceCliComp::__OnConnect(
    bbt::network::Errcode err,
    bbt::network::interface::INetConnectionSPtr conn)
{
    /* 不管成功还是失败，先设置连接中状态位 */
    m_is_connecting = false;

    if (err.IsErr()) {
        GAME_EXT1_LOG_ERROR(err.CWhat());
        return;
    }

    auto network_obj = plugin::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    if (network_obj == nullptr) {
        GAME_EXT1_LOG_ERROR("can`t found Gameobject Tid=%d", (int)EM_ENTITY_TYPE_NETWORK);
        return;
    }

    auto comp = network_obj->GetComponent(EM_COMPONENT_TYPE_SERVER);
    if (comp == nullptr) {
        GAME_EXT1_LOG_ERROR("can`t found Component Tid=%d", (int)EM_COMPONENT_TYPE_SERVER);
        return;
    }

    auto network_comp = std::dynamic_pointer_cast<plugin::ecs::network::Server>(comp); 

    /* 连接建立成功，加入ConnMgr进行管理 */
    auto dbconn = std::make_shared<plugin::ecs::network::DBServiceConnection>(
        std::dynamic_pointer_cast<bbt::network::libevent::Connection>(conn), m_cfg.timeout);
    dbconn->SetOnClose([this](bbt::network::ConnId connid){
        m_connid = 0;
        GAME_EXT1_LOG_WARN("db service connection closed! connid=%d", connid);
    });
    
    GAME_EXT1_LOG_DEBUG("db service connection! connid=%d", dbconn->GetConnId());
    network_comp->AddConnect(dbconn);
    m_last_heartbeat_timestamp = bbt::clock::now<>();
    m_connid = dbconn->GetConnId();
}

std::shared_ptr<DBServiceConnection> DBServiceCliComp::GetConnect()
{
    auto network_obj = plugin::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    std::shared_ptr<plugin::ecs::network::Server> network_comp = nullptr;

    auto comp = network_obj->GetComponent(EM_COMPONENT_TYPE_SERVER);
    if ( comp == nullptr || (network_comp= std::dynamic_pointer_cast<plugin::ecs::network::Server>(comp)) == nullptr) {
        GAME_EXT1_LOG_ERROR("can`t found Component Tid=%d", (int)EM_COMPONENT_TYPE_SERVER);
        return nullptr;
    }

    auto connect = network_comp->GetConnectById(m_connid);
    return std::static_pointer_cast<DBServiceConnection>(connect);
}


//////////////////////////////////////////////////////////////////////////
util::errcode::ErrOpt DBServiceCliCompSystem::Init(engine::ecs::GameObjectSPtr gameobj, DBServiceCliCfg* conn_cfg)
{
    if (gameobj == nullptr) {
        GAME_EXT1_LOG_ERROR("gameobj is null!");
        return util::errcode::ErrCode("gameobj is null!", util::errcode::ErrType::CommonErr);
    }

    auto isok = gameobj->AddComponent<plugin::ecs::network::DBServiceCliComp>(*conn_cfg);
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

}