#pragma once
#include <map>
#include <set>
#include <string>
#include <bbt/ecs/scene/Scene.hpp>
#include <bbt/ecs/filter/TagSet.hpp>

namespace bbt::ecs
{
class Entity:
    public std::enable_shared_from_this<Entity>,
    public bbt::core::util::MemberBase<EntityId, Entity>
{
    friend class EntityMgr;
    friend class bbt::ecs::Scene;
public:
    explicit        Entity(int gobj_type);
    virtual         ~Entity() = 0;


    template<class TComponent, typename ...Args>
    bool            AddComponent(Args ...args);
    /* 插入一个组件, 如果组件已经存在，返回false，否则true */
    bool            AddComponent(ComponentSPtr component);
    /* 获取一个组件，如果不存在返回nullptr */
    ComponentSPtr   GetComponent(ComponentTemplateId component_name) const;
    template<class TComponent>
    std::shared_ptr<TComponent> GetComponent() const;
    size_t          GetComponentCount() const;
    /* 删除一个组件，如果不存在返回nullptr */
    ComponentSPtr   DelComponent(ComponentTemplateId component_name);
    /* 游戏对象的类型 */
    GameObjectTemplateId Type();
    EntityId    GetId();
    std::string     GetName() const;
    /* 根据游戏对象的名字，在当前游戏对象的子游戏对象中找到游戏对象 */
    EntitySPtr  GetGameObject(EntityId id);
    // TODO
    /* 根据游戏对象的名字，递归的在子游戏对象中找到游戏对象 */
    // EntitySPtr  RecursionGetGameObject(std::string gameobj_name);

    /* 动态的添加一个子游戏对象，成功返回true，失败返回false */
    bool            MountChild(EntitySPtr gameobj);
    TagSet&         GetTagSet();
    SceneSPtr       GetScene() const;
    EntityMgrSPtr   GetEntityMgr() const;

    std::pair<bool, EntitySPtr> UnMountChild(EntityId gameobj_id);
    std::string     Dbg_GameObjectTree() const;
protected:
    virtual void    OnFatherDead() const;

    /**
     * @brief 游戏对象调用 Update 前调用
     */
    virtual void    OnPreUpdate() {}

    /**
     * @brief 游戏对象调用 Update 之后调用
     */
    virtual void    OnUpdate();

    virtual void    OnMountChild(EntitySPtr gameobj);
    virtual void    OnUnMountChild(EntitySPtr gameobj);

private:
    /**
     * @brief 此函数被调用时会自上而下的遍历所有的子节点并Update
     * 同时触发OnUpdate去更新
     */
    virtual void    Update() final;
    virtual void    UpdateComponent() final;
    virtual bool    HasGameobj(EntityId id) const final;
    std::string     __DbgTree(int level) const;

private:
    /* 游戏对象的类型，每个游戏对象的实例都需要对应与一个已经定义的GameObjType，否则会导致未知行为 */
    const GameObjectTemplateId                      m_gobj_type{-1};
    /* 每个游戏对象都可以保存着一些组件 */
    std::unordered_map<ComponentTemplateId, ComponentSPtr>
                                                    m_component_map;
    /* 游戏对象是递归的，也就是说一个游戏对象可以作为节点来包含一些子对象 */
    std::map<EntityId, EntitySPtr>          m_childs;
    /* 父游戏对象 */
    std::map<EntityId, EntityWKPtr>         m_fathers;
    TagSet                                          m_tag_set;
};


template<class TComponent, typename ...Args>
bool Entity::AddComponent(Args ...args)
{
    auto component_mgr = GetScene()->GetComponentMgr();
    if (!component_mgr)
        return false;

    auto comp = component_mgr->Create<TComponent>(args...);
    return AddComponent(comp);
}

template<class TComponent>
std::shared_ptr<TComponent> Entity::GetComponent() const
{
    return std::dynamic_pointer_cast<TComponent>(GetComponent(BBT_REFLEX_GET_TYPEID(TComponent)));
}

} // namespace bbt::ecs
