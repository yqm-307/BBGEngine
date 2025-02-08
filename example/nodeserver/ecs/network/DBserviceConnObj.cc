#include "nodeserver/ecs/network/DBServiceConnObj.hpp"
#include "nodeserver/ecs/network/DBServiceCliComp.hpp"
#include "plugin/ecs/Define.hpp"

namespace share::ecs::network
{

DBServiceConnObj::DBServiceConnObj():
    engine::ecs::GameObject(EM_ENTITY_TYPE_DB_SERVICE_CLIENT)
{
}

DBServiceConnObj::~DBServiceConnObj()
{
}

void DBServiceConnObj::OnUpdate()
{
}

} // namespace share::ecs::network
