#include "plugin/ecs/globalmgr/GlobalMgr.hpp"


namespace plugin::ecs::globalmgr
{

GlobalMgr::GlobalMgr():
    engine::ecs::Entity(EM_ENTITY_TYPE_DB_GLOBAL_MGR)
{

}

GlobalMgr::~GlobalMgr()
{
}

bool GlobalMgr::AddGlobalInst(engine::ecs::EntitySPtr gameobj)
{
    if (gameobj == nullptr)
        return false;

    auto mount_succ = MountChild(gameobj);
    if (!mount_succ)
        return false;
    
    auto [it, succ] = m_global_instance_map.insert(std::pair(gameobj->Type(), gameobj));

    return succ;
}

std::pair<bool, engine::ecs::EntitySPtr> GlobalMgr::DelGlobalInst(engine::ecs::GameObjectTemplateId tid)
{
    auto it = m_global_instance_map.find(tid);
    if (it == m_global_instance_map.end())
        return {false, nullptr};
    
    auto [succ, obj] = UnMountChild(it->second->GetId());
    if (!succ)  
        return {false, nullptr};
    
    return {true, obj};
}

engine::ecs::EntitySPtr GlobalMgr::GetInstByTid(engine::ecs::GameObjectTemplateId tid)
{
    auto it = m_global_instance_map.find(tid);
    if (it == m_global_instance_map.end())
        return nullptr;

    return it->second;
}


} // namespace share::ecs::globalmgr

