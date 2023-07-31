#include "share/scene/Scene.hpp"

namespace game::share::scene
{

Scene::Scene()
    :m_all_obj([](const std::string& key){ return (size_t)(std::hash<std::string>()(key)%Default_Hash_Bucket_Num); }, nullptr)
{

}

Scene::~Scene()
{
}

}