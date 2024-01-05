#include <cmath>
#include "share/ecs/aoi/entity/Aoi.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"
#include "engine/ecs/component/ComponentMgr.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::aoi
{


Aoi::Aoi()
    :GameObject(share::ecs::EM_ENTITY_TYPE_AOI)
{
}

Aoi::~Aoi()
{
}

void Aoi::OnUpdate()
{
}

} // namespace engine::ecs::entity::aoi
