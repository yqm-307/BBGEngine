#pragma once
#include <engine/ecs/entity/Entity.hpp>
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::globalmgr
{

class GlobalMgr:
    public engine::ecs::Entity
{
    GameObjectClassMetaInfo;
public:
    virtual ~GlobalMgr();

    bool                        AddGlobalInst(engine::ecs::EntitySPtr gameobj);
    std::pair<bool, engine::ecs::EntitySPtr> 
                                DelGlobalInst(engine::ecs::GameObjectTemplateId tid);
    engine::ecs::EntitySPtr GetInstByTid(engine::ecs::GameObjectTemplateId tid);
protected:
    GlobalMgr();

private:
    std::unordered_map<engine::ecs::GameObjectTemplateId, engine::ecs::EntitySPtr>
                    m_global_instance_map;
};

} // namespace share::ecs::globalmgr
