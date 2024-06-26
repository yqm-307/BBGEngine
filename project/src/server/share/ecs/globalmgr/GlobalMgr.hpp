#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>
#include "share/ecs/Define.hpp"

namespace share::ecs::globalmgr
{

class GlobalMgr:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    virtual ~GlobalMgr();

    bool                        AddGlobalInst(engine::ecs::GameObjectSPtr gameobj);
    std::pair<bool, engine::ecs::GameObjectSPtr> 
                                DelGlobalInst(engine::ecs::GameObjectTemplateId tid);
    engine::ecs::GameObjectSPtr GetInstByTid(engine::ecs::GameObjectTemplateId tid);
protected:
    GlobalMgr();

private:
    std::unordered_map<engine::ecs::GameObjectTemplateId, engine::ecs::GameObjectSPtr>
                    m_global_instance_map;
};

} // namespace share::ecs::globalmgr
