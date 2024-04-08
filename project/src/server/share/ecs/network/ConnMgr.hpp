#pragma once
#include "engine/ecs/component/Component.hpp"

namespace share::ecs::network
{

class ConnMgr:
    public engine::ecs::Component
{
public:
    ~ConnMgr();
protected:
    ConnMgr();
private:
};

}