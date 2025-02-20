#pragma once
#include <memory>
#include <engine/scene/Scene.hpp>
#include "plugin/ecs/Define.hpp"
#include "plugin/ecs/globalmgr/GlobalMgr.hpp"

namespace plugin::scene
{

extern inline std::unique_ptr<engine::ecs::Scene>& MainScene()
{
    static std::unique_ptr<engine::ecs::Scene> g_scene = nullptr;
    return g_scene;
}

extern inline engine::ecs::GameObjectSPtr GetGlobalInstByTid(engine::ecs::GameObjectTemplateId tid)
{
    // auto objs = MainScene()->GetGameObjectMgr()->GetGameObject(plugin::ecs::EM_ENTITY_TYPE_DB_GLOBAL_MGR);
    // if (objs.size() != 1)
    //     return nullptr;
    
    // auto global_mgr = std::dynamic_pointer_cast<plugin::ecs::globalmgr::GlobalMgr>(objs[0]);
    // if (global_mgr == nullptr)
    //     return nullptr;

    return nullptr;
}

extern inline bool RegistGlobalInst(engine::ecs::GameObjectSPtr obj)
{
    // auto objs = MainScene()->GetGameObjectMgr()->GetGameObject(plugin::ecs::EM_ENTITY_TYPE_DB_GLOBAL_MGR);
    // if (objs.size() != 1)
    //     return false;
    
    // auto global_mgr = std::dynamic_pointer_cast<plugin::ecs::globalmgr::GlobalMgr>(objs[0]);
    // if (global_mgr == nullptr)
    //     return false;
    
    return false;
}

}