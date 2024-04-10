#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/network/ConnMgr.hpp"
#include "share/ecs/Define.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

ConnMgr::~ConnMgr()
{

}

ConnMgr::ConnMgr(const ServerCfg& cfg):
    engine::ecs::Component(EM_COMPONENT_TYPE_CONN_MGR),
    m_cfg(cfg)
{
}

bool ConnMgr::Init()
{
    auto parent = GetParentObject();
    if (parent == nullptr)
        return false;

    auto network_comp = std::static_pointer_cast<NetworkComponent>(parent->GetComponent(EM_COMPONENT_TYPE_NETWORK));
    if (network_comp == nullptr)
        return false;
    
    network_comp->SetListenAddr(m_cfg.ip.c_str(), m_cfg.port);
    network_comp->SetOnAccept([this](auto err, auto new_conn){
        OnAcceptAndInitConn(err, new_conn);
    });

    return true;
}

void ConnMgr::OnAcceptAndInitConn(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn)
{
    auto conn = std::make_shared<network::Connection>(this, new_conn, 1000);
    if (!AddConnect(conn)) {
        GAME_EXT1_LOG_ERROR("add connect failed!");
        return;
    }
}

bool ConnMgr::DelConnect(bbt::network::ConnId conn)
{
    auto it = m_conn_map.find(conn);
    if (it != m_conn_map.end()) {
        return false;
    }

    m_conn_map.erase(it);
    return true;
}

bool ConnMgr::AddConnect(std::shared_ptr<Connection> conn)
{
    if (conn == nullptr)
        return false;

    auto [it, succ] = m_conn_map.insert(std::make_pair(conn->GetConnId(), conn));
    return succ;
}

void ConnMgr::OnTimeout(Connection* conn)
{
    if (!DelConnect(conn->GetConnId())) {
        GAME_EXT1_LOG_ERROR("[ConnMgr::OnTimeout] connect timeout!");
    }
}

}