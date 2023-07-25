#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>

namespace game::share::ecs
{

class GameObject;

class Component: 
    public bbt::templateutil::BaseType<Component>
{
public:
    Component(){}


    // interface
    //--------------------------------------------
    virtual ~Component() = 0;
    virtual void OnAddComponent() = 0;
    virtual void OnDelComponent() = 0;
    virtual void OnUpdate() = 0;
    //--------------------------------------------

    virtual void OnCreate(){}
    virtual void OnDestory(){}
    const std::string& GetName() const;
protected:
    void SetName(std::string component);
private:
    std::string     m_name;
};

}