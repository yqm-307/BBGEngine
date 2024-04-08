#pragma once
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/network/Connection.hpp"

namespace share::ecs::network
{

class ConnMgr:
    public engine::ecs::Component
{
public:
    ~ConnMgr();
    bool Init();
    
protected:
    ConnMgr();
private:
    std::unordered_map<engine::ecs::ComponentId, std::shared_ptr<Connection>>    m_conn_map;
};

}