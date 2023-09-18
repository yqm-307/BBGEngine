#pragma once
#include <map>
#include <set>
#include <string>
#include "engine/ecs/GameObjectDef.hpp"
#include "engine/ecs/Component.hpp"
#include "util/typedef/NamespaceType.hpp"

namespace game::share::ecs
{
class GameObject;
SmartPtrTypeDef(GameObject);


class GameObject:
    public std::enable_shared_from_this<GameObject>
{
public:
    explicit GameObject(GameObjType gobj_type);
    virtual ~GameObject() = 0;
    virtual void OnCreate(){};
    virtual void OnDestory(){};
    virtual void OnUpdate() = 0;
    
    /* 插入一个组件, 如果组件已经存在，返回false，否则true */
    bool            AddComponent(ComponentSPtr component);
    /* 获取一个组件，如果不存在返回nullptr */
    ComponentSPtr   GetComponent(ComponentTemplateId component_name);
    /* 删除一个组件，如果不存在返回nullptr */
    ComponentSPtr   DelComponent(ComponentTemplateId component_name);
    /* 游戏对象的类型 */
    GameObjType     Type();

    /* 根据游戏对象的名字，在当前游戏对象的子游戏对象中找到游戏对象 */
    GameObjectSPtr  GetGameObject(const std::string& gameobj_name);

    /* 根据游戏对象的名字，递归的在子游戏对象中找到游戏对象 */
    // GameObjectSPtr  RecursionGetGameObject(std::string gameobj_name);

    /* 动态的添加一个子游戏对象，成功返回true，失败返回false */
    bool DynamicAddChild(GameObjectSPtr gameobj);

    const std::string& GetName() const;
private:

    bool HasGameobj(const std::string& name) const;

    std::string m_gameobj_name;

    /* 游戏对象的类型，每个游戏对象的实例都需要对应与一个已经定义的GameObjType，否则会导致未知行为 */
    GameObjType m_gobj_type;

    /* 每个游戏对象都可以保存着一些组件 */
    std::map<ComponentTemplateId, ComponentSPtr>  m_component_map;

    /* 游戏对象时递归的，也就是说一个游戏对象可以作为节点来包含一些子对象 */
    std::map<std::string, GameObjectSPtr>   m_childs;

    /* 父游戏对象 */
    GameObjectWKPtr m_partner;
};

}
