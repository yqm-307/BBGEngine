#include "engine/ecs/gameobject/GameObjectMgr.hpp"
#include "util/log/Log.hpp"


namespace engine::ecs
{

const std::unique_ptr<GameObjectMgr>& GameObjectMgr::GetInstance()
{
    static std::unique_ptr<GameObjectMgr> _instance{nullptr};
    if(_instance == nullptr)
        _instance = std::unique_ptr<GameObjectMgr>(new GameObjectMgr());

    return _instance;
}

GameObjectMgr::GameObjectMgr() 
{
}

GameObjectMgr::~GameObjectMgr() 
{
    // FIXME 
    /**
     * 需要主动触发残留对象的释放
     * 
     * 否则等map自动释放gameobject时会将m_gameobject_map中的树节点一并释放掉，导致double free
     * 逻辑如下:
     * 
     * 
     * 如果不做提前销毁: 
     * clear 
     *          release node -->    // 删除节点
     *                      release GameObject -->  // 原意是对象释放，从mgr中移除管理
     *                              release node(成功的) --> // 删除节点 (double free)
     *          release node -->
     * clear
     */
    while(!m_gameobject_map.empty())
    {
        m_gameobject_map.begin()->second = nullptr;
    }
    GAME_EXT1_LOG_INFO("release all item. remain item num=%d", m_gameobject_map.size());
    DebugAssert(m_gameobject_map.size() == 0);
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

bool GameObjectMgr::OnDestoryGameObject(KeyType key)
{
    auto it = m_gameobject_map.find(key);
    if(it == m_gameobject_map.end())
    {
        return false;
    }

    m_gameobject_map.erase(it);
    return true;
}



}
