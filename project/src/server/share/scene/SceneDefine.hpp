#pragma once
#include <memory>
#include <engine/scene/Scene.hpp>
#include "share/ecs/Define.hpp"
#include "share/ecs/globalmgr/GlobalMgr.hpp"

namespace share::scene
{

extern inline std::unique_ptr<engine::scene::Scene>& MainScene()
{
    static std::unique_ptr<engine::scene::Scene> g_scene = nullptr;
    return g_scene;
}

extern inline engine::ecs::GameObjectSPtr GetGlobalInstByTid(engine::ecs::GameObjectTemplateId tid)
{
    auto objs = MainScene()->GetGameObject(share::ecs::EM_ENTITY_TYPE_DB_GLOBAL_MGR);
    if (objs.size() != 1)
        return nullptr;
    
    auto global_mgr = std::dynamic_pointer_cast<share::ecs::globalmgr::GlobalMgr>(objs[0]);
    if (global_mgr == nullptr)
        return nullptr;

    return global_mgr->GetInstByTid(tid);
}

extern inline bool RegistGlobalInst(engine::ecs::GameObjectSPtr obj)
{
    auto objs = MainScene()->GetGameObject(share::ecs::EM_ENTITY_TYPE_DB_GLOBAL_MGR);
    if (objs.size() != 1)
        return false;
    
    auto global_mgr = std::dynamic_pointer_cast<share::ecs::globalmgr::GlobalMgr>(objs[0]);
    if (global_mgr == nullptr)
        return false;
    
    return global_mgr->AddGlobalInst(obj);
}

}