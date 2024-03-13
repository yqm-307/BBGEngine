#pragma once

namespace engine::ecs
{

class System
{
public:
    System(){}
    virtual ~System() = 0;
    virtual void OnDispatch() = 0;
private:
    
};

}