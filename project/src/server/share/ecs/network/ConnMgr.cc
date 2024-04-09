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
}

void ConnMgr::OnAcceptAndInitConn(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn)
{
    auto conn = std::make_shared<Connection>();
    
}


}