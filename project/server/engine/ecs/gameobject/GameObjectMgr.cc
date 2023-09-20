#include "engine/ecs/gameobject/GameObjectMgr.hpp"


namespace game::share::ecs
{

const std::unique_ptr<GameObjectMgr>& GameObjectMgr::GetInstance()
{
    std::unique_ptr<GameObjectMgr>  _instance{nullptr};
    if(_instance == nullptr)
        _instance = std::unique_ptr<GameObjectMgr>(new GameObjectMgr());

    return _instance;
}

GameObjectMgr::GameObjectMgr() 
{
}

GameObjectMgr::~GameObjectMgr() 
{
}

typename GameObjectMgr::Result GameObjectMgr::Search(KeyType key)
{
    auto it_obj = m_gameobject_map.find(key);
    if(it_obj == m_gameobject_map.end())
        return {nullptr, false};

    return {it_obj->second, true};
}

bool GameObjectMgr::IsExist(KeyType key)
{
    return !(m_gameobject_map.find(key) == m_gameobject_map.end());
}

bool GameObjectMgr::OnInitGameObject(KeyType key, ValueType value)
{
    auto [_, isok] = m_gameobject_map.insert(std::make_pair(key, value));
    return isok;
}


}
