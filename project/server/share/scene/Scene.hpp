#pragma once
#include <bbt/templateutil/Noncopyable.hpp>
#include "share/ecs/GameObject.hpp"
#include "util/hashmap/Hashmap.hpp"

namespace game::share::scene
{

const int Default_Hash_Bucket_Num = 128;

class Scene:
    public bbt::templateutil::noncopyable
{
public:
    typedef util::hashmap::Hashmap<std::string, ecs::GameObject::SPtr> GameObjMap;
    
    Scene();
    ~Scene();

    virtual void OnUpdate() = 0;
private:
    GameObjMap  m_all_obj;
};

}