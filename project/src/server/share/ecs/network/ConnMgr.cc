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

}