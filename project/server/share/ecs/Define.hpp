#pragma once

namespace share::ecs
{

enum emEntityType: int {

//------------------------> 无实体对象 <------------------------//
    EM_ENTIRY_TYPE_NONE                     = 0,    // 空对象
    EM_ENTITY_TYPE_AOI                      = 1,    // aoi
    EM_ENTITY_TYPE_PLAYER_MGR               = 2,    // 玩家管理器


//------------------------> 实体对象 <------------------------//
    EM_ENTITY_TYPE_HAS_ENTITY               = 1000000,
    EM_ENTITY_TYPE_PLAYER                   = 1000001,    // 玩家

};

} // namespace share::ecs