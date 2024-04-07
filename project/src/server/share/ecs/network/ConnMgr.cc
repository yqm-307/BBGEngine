#include "share/ecs/network/ConnMgr.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::network
{

ConnMgr::~ConnMgr()
{

}

ConnMgr::ConnMgr():
    engine::ecs::GameObject(EM_ENTITY_TYPE_CONN_MANAGER)
{

}

}