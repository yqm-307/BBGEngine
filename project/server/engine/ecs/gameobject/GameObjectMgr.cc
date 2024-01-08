#include "engine/ecs/gameobject/GameObjectMgr.hpp"
#include "engine/ecs/gameobject/GameObject.hpp"
#include "util/log/Log.hpp"
#include <tuple>

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
    :m_gameobject_info_map(
        [](const ComponentTemplateId& key){ return (size_t)(key%8);},
        GameObjectInfo("", -1))
{
}

GameObjectMgr::~GameObjectMgr() 
{
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
        auto&& wkptr = m_gameobject_map.begin()->second;
        auto sptr = wkptr.lock();
        if(sptr && sptr.use_count() > 1)
        {
            GAME_BASE_LOG_WARN("[GameObjectMgr::~GameObjectMgr] type=%d use_count=%d", 
                sptr->Type(),
                sptr.use_count());
        }
        sptr = nullptr;
    }
    GAME_BASE_LOG_DEBUG("[GameObjectMgr::~GameObjectMgr] release all item. remain item num=%d", m_gameobject_map.size());
    DebugAssert(m_gameobject_map.size() == 0);
}

typename GameObjectMgr::Result GameObjectMgr::Search(KeyType key)
{
    auto it_obj = m_gameobject_map.find(key);
    if(it_obj == m_gameobject_map.end())
        return {nullptr, false};

    return {(it_obj->second).lock(), true};
}

bool GameObjectMgr::IsExist(KeyType key)
{
    return !(m_gameobject_map.find(key) == m_gameobject_map.end());
}

std::string GameObjectMgr::GetName(GameObjectTemplateId tid) const
{
    auto [info, isok] = m_gameobject_info_map.Find(tid);
    
    if (isok) {
        return std::get<0>(info);
    }

    return "";
}

bool GameObjectMgr::OnMemberCreate(MemberPtr member_base)
{
    auto gid = member_base->GetMemberId();
    auto it = m_gameobject_map.find(gid);
    if(it != m_gameobject_map.end())
        return false;

    auto [_, isok] = m_gameobject_map.insert(std::make_pair(gid, member_base));
    return isok;
}

bool GameObjectMgr::OnMemberDestory(KeyType key)
{
    auto it = m_gameobject_map.find(key);
    if(it == m_gameobject_map.end())
        return false;

    auto idx = m_gameobject_map.erase(key);
    return (idx > 0);
}

GameObjectMgr::KeyType 
GameObjectMgr::GenerateKey(MemberPtr member_base)
{
    return engine::ecs::GenerateGameObjectID();
}

bool GameObjectMgr::InitTemplateInfo(std::initializer_list<GameObjectInfo> list)
{
    static bool is_inited = false;
    if( is_inited )
        return false;

    is_inited = true;

    for (auto&& item : list)
    {
        GameObjectTemplateId id = std::get<1>(item);
        AssertWithInfo(m_gameobject_info_map.Insert(id, item), "check gameobject info register is right!");
    }

    return true;    
}


}
