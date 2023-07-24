#pragma once
#include <map>
#include <set>
#include <string>
#include "share/ecs/Component.hpp"

namespace game::share::ecs
{

enum GameObjType: int
{
    Player = 1001,
};


class GameObject: public bbt::templateutil::BaseType<GameObject>
{
public:
    GameObject(GameObjType gobj_type);
    virtual ~GameObject() = 0;
    virtual void OnCreate(){};
    virtual void OnDestory(){};
    virtual void OnUpdate() = 0;
    
    /* 插入一个组件, 如果组件已经存在，返回false，否则true */
    bool            AddComponent(Component::SPtr component);
    /* 获取一个组件，如果不存在返回nullptr */
    Component::SPtr GetComponent(std::string component_name);
    /* 删除一个组件，如果不存在返回nullptr */
    Component::SPtr DelComponent(std::string component_name);
    /* 游戏对象的类型 */
    GameObjType     Type();
private:
    GameObjType                             m_gobj_type;
    std::map<std::string, Component::SPtr>  m_component_map;
    std::set<std::string>                   m_components;
};

}

#include "share/ecs/GameObjectT.hpp"