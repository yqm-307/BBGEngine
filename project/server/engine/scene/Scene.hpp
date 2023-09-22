#pragma once
#include <bbt/templateutil/Noncopyable.hpp>
#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/hashmap/Hashmap.hpp"

namespace engine::scene
{

const int Default_Hash_Bucket_Num = 128;

// FIXME 也许场景也是一个游戏对象
class Scene:
    public engine::ecs::GameObject
{
public:
    typedef std::map<std::string, engine::ecs::GameObjectSPtr> GameObjMap;
    typedef std::pair<engine::ecs::GameObjectSPtr, bool> Result;
    Scene();
    virtual ~Scene() = 0;

    virtual void Update() final;
    virtual bool MountGameObject(engine::ecs::GameObjectSPtr) final;
    virtual bool UnMountGameObject(engine::ecs::GameObjectSPtr) final; 
protected:
    virtual void OnUpdate() = 0;

    /* 场景根节点的 游戏对象数组 */
    std::vector<engine::ecs::GameObjectSPtr> m_root_gobjs;
};

}