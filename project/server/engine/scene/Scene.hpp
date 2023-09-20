#pragma once
#include <bbt/templateutil/Noncopyable.hpp>
#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/hashmap/Hashmap.hpp"

namespace game::share::scene
{

const int Default_Hash_Bucket_Num = 128;

class Scene
{
public:
    typedef std::map<std::string, ecs::GameObjectSPtr> GameObjMap;
    typedef std::pair<ecs::GameObjectSPtr, bool> Result;
    Scene();
    virtual ~Scene() = 0;

    virtual void OnUpdate() = 0;
};

}