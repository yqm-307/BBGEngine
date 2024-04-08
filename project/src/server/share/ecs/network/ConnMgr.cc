#include "engine/ecs/gameobject/GameObject.hpp"
#include "share/ecs/network/ConnMgr.hpp"
#include "share/ecs/Define.hpp"
#include "share/ecs/network/NetworkComponent.hpp"

namespace share::ecs::network
{

ConnMgr::~ConnMgr()
{

}

ConnMgr::ConnMgr():
    engine::ecs::Component(EM_COMPONENT_TYPE_CONN_MGR)
{
}

bool ConnMgr::Init()
{
    auto parent = GetParentObject();
    if (parent == nullptr)
        return false;

    auto network_comp = parent->GetComponent(EM_COMPONENT_TYPE_NETWORK);
    if (network_comp == nullptr)
        return false;
    
}

}