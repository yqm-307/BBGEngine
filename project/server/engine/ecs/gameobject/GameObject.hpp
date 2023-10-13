#pragma once
#include <map>
#include <set>
#include <string>
#include "engine/ecs/gameobject/GameObjectMgr.hpp"
#include "engine/ecs/component/Component.hpp"
#include "util/typedef/NamespaceType.hpp"

namespace engine
{
namespace scene
{
class Scene;
SharedWithUniqueDef(Scene);
}

namespace ecs    
{
class GameObject;
// SmartPtrTypeDef(GameObject);


class GameObject:
    public std::enable_shared_from_this<GameObject>
{
    friend class GameObjectMgr;
    friend class engine::scene::Scene;
public:
    explicit GameObject(int gobj_type);
    virtual ~GameObject() = 0;


    /* 插入一个组件, 如果组件已经存在，返回false，否则true */
    bool            AddComponent(ComponentSPtr component);
    /* 获取一个组件，如果不存在返回nullptr */
    ComponentSPtr   GetComponent(ComponentTemplateId component_name);
    /* 删除一个组件，如果不存在返回nullptr */
    ComponentSPtr   DelComponent(ComponentTemplateId component_name);
    /* 游戏对象的类型 */
    int             Type();

    GameObjectId    GetId();

    /* 根据游戏对象的名字，在当前游戏对象的子游戏对象中找到游戏对象 */
    GameObjectSPtr  GetGameObject(GameObjectId id);

    // TODO
    /* 根据游戏对象的名字，递归的在子游戏对象中找到游戏对象 */
    // GameObjectSPtr  RecursionGetGameObject(std::string gameobj_name);

    /* 动态的添加一个子游戏对象，成功返回true，失败返回false */
    bool MountChild(GameObjectSPtr gameobj);

    // TODO
    // bool UnMountChild(GameObjectSPtr gameobj);

    const std::string& GetName() const;
protected:
    virtual void OnFatherDead() = 0;

    /**
     * @brief 游戏对象调用 Update 前调用
     */
    virtual void OnPreUpdate() {}

    /**
     * @brief 游戏对象调用 Update 之后调用
     */
    virtual void OnUpdate() = 0;
    

private:
    /**
     * @brief 此函数被调用时会自上而下的遍历所有的子节点并Update
     * 同时触发OnUpdate去更新
     */
    void Update();
    void SetId(GameObjectId id);
    bool HasGameobj(GameObjectId id) const;
    void OnCreate();
    void OnDestory();

    GameObjectId    m_id{-1};

    std::string m_gameobj_name{""};

    /* 游戏对象的类型，每个游戏对象的实例都需要对应与一个已经定义的GameObjType，否则会导致未知行为 */
    const int m_gobj_type{-1};

    /* 每个游戏对象都可以保存着一些组件 */
    std::map<ComponentTemplateId, ComponentSPtr>  m_component_map;

    /* 游戏对象是递归的，也就是说一个游戏对象可以作为节点来包含一些子对象 */
    std::map<GameObjectId, GameObjectSPtr> m_childs;

    /* 父游戏对象 */
    std::map<GameObjectId, GameObjectWKPtr> m_fathers;

    engine::scene::SceneWKPtr m_scene;
};

} // namespace engine::ecs

} // namespace engine
