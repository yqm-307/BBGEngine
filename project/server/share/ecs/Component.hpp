#pragma once
#include <string>
#include <bbt/templateutil/BaseType.hpp>

namespace game::share::ecs
{

class GameObject;

class Component: public bbt::templateutil::BaseType<Component>
{
public:
    Component(){}
    virtual ~Component() = 0;

    virtual void OnCreate(){}
    virtual void OnDestory(){}
    virtual void OnAddComponent(GameObject) = 0;
    virtual void OnDelComponent(GameObject) = 0;
    virtual void OnUpdate() = 0;
    const std::string& GetName() const;
protected:
    void SetName(std::string component);
private:
    std::string     m_name;
};

}