#pragma once

namespace share::ecs
{

enum emEntityType: int {

//------------------------> 无实体对象 <------------------------//
    EM_ENTIRY_TYPE_NONE                     = 0,    // 空对象
    EM_ENTITY_TYPE_AOI                      = 1,    // aoi
    EM_ENTITY_TYPE_PLAYER_MGR               = 2,    // 玩家管理器
    EM_ENTITY_TYPE_COMM_SCENE               = 3,    // 通用空场景
    EM_ENTITY_TYPE_GAMESERVER_NETWORK       = 4,    // gs 网络层


//------------------------> 实体对象 <------------------------//
    EM_ENTITY_TYPE_HAS_ENTITY               = 1000000,
    EM_ENTITY_TYPE_PLAYER                   = 1000001,    // 玩家


//------------------------> 测试对象 <------------------------//
    EM_ENTITY_TYPE_TESTOBJ_1                = 1001001,  // 测试对象1

};

} // namespace share::ecs