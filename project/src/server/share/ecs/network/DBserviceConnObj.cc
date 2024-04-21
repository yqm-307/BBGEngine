#include "share/ecs/network/DBServiceConnObj.hpp"
#include "share/ecs/network/DBServiceCliComp.hpp"
#include "share/ecs/Define.hpp"

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
