#pragma once
#include <bbt/base/templateutil/Noncopyable.hpp>
#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/hashmap/Hashmap.hpp"

namespace engine::scene
{

const int Default_Hash_Bucket_Num = 128;


class Scene
{
    typedef std::pair<engine::ecs::GameObjectSPtr, bool> Result;
public:
    Scene();
    virtual ~Scene() = 0;

    virtual void Update() final;
    virtual bool MountGameObject(engine::ecs::GameObjectSPtr) final;
    virtual Result UnMountGameObject(engine::ecs::GameObjectSPtr) final; 
    virtual Result UnMountGameObject(engine::ecs::GameObjectId) final;
    virtual size_t GetChildNum() final;
protected:
    virtual void OnUpdate() = 0;

    /* 场景根节点的 游戏对象数组 */
    std::vector<engine::ecs::GameObjectSPtr> m_root_gobjs;
};

}