#include <engine/ecs/filter/EntityFilter.hpp>
#include <engine/ecs/entity/EntityMgr.hpp>
#include <engine/ecs/entity/Entity.hpp>
#include <util/log/Log.hpp>
#include <tuple>

namespace engine::ecs
{

EntityMgr::EntityMgr(SceneSPtr scene)
    :m_gameobject_info_map(
        [](const ComponentTemplateId& key){ return (size_t)(key%8);},
        GameObjectInfo("", -1)),
    m_scene(scene)
{
}

EntityMgr::~EntityMgr() 
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

typename EntityMgr::Result EntityMgr::Search(KeyType key)
{
    auto it_obj = m_gameobject_map.find(key);
    if(it_obj == m_gameobject_map.end())
        return {nullptr, false};

    return {(it_obj->second).lock(), true};
}

bool EntityMgr::IsExist(KeyType key)
{
    return !(m_gameobject_map.find(key) == m_gameobject_map.end());
}

size_t EntityMgr::ObjCount() const
{
    return m_gameobject_map.size();
}


std::string EntityMgr::GetName(GameObjectTemplateId tid) const
{
    auto [info, isok] = m_gameobject_info_map.Find(tid);
    
    if (isok) {
        return std::get<0>(info);
    }

    return "";
}

bool EntityMgr::OnMemberCreate(MemberPtr member_base)
{
    auto gid = member_base->GetMemberId();
    auto it = m_gameobject_map.find(gid);
    if(it != m_gameobject_map.end())
        return false;

    auto [_, isok] = m_gameobject_map.insert(std::make_pair(gid, member_base));
    return isok;
}

bool EntityMgr::OnMemberDestory(KeyType key)
{
    auto it = m_gameobject_map.find(key);
    if(it == m_gameobject_map.end())
        return false;

    auto idx = m_gameobject_map.erase(key);
    return (idx > 0);
}

EntityMgr::KeyType 
EntityMgr::GenerateKey(MemberPtr member_base)
{
    return engine::ecs::GenerateGameObjectID();
}

bool EntityMgr::InitTemplateInfo(std::initializer_list<GameObjectInfo> list)
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

int EntityMgr::GetGameobjectByFilter(std::vector<EntityWKPtr>& gameobjects, std::shared_ptr<EntityFilter> filter)
{
    int count = 0;
    if (filter == nullptr)
        return count;

    for(auto&& [_, wkptr] : m_gameobject_map)
    {
        auto sptr = wkptr.lock();
        if(sptr == nullptr)
            continue;

        if(filter->Filter(sptr))
        {
            gameobjects.push_back(sptr);
            count++;
        }
    }

    return count;
}

SceneSPtr EntityMgr::GetScene() const
{
    return m_scene.lock();
}

}
