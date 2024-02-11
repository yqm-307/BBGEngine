#pragma once
#include <bbt/base/Attribute.hpp>
#include "engine/ecs/component/ComponentMgr.hpp"
#include "engine/ecs/gameobject/GameObjectMgr.hpp"


namespace share::ecs
{

enum emEntityType: int {

//------------------------> 无实体对象 <------------------------//
    EM_ENTITY_TYPE_GAMEOBJECT               = 0,    // 空对象
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


enum emComponentType: int {
//------------------------> 特殊组件 <------------------------//
    EM_COMPONENT_TYPE_INVALID               = -1,   // 非法组件
    EM_COMPONENT_TYPE_NONE                  = 0,    // 空组件

//------------------------> 常规组件 <------------------------//
    EM_COMPONENT_TYPE_AOI                   = 1,    // aoi组件
    EM_COMPONENT_TYPE_AOI_OBJECT            = 2,    // aoi object组件

//------------------------> 测试组件 <------------------------//
    EM_COMPONENT_TYPE_TESTCOMP_1            = 1001001,  // 测试组件
};

class EcsInitHelper
{
public:
    EcsInitHelper(){
        G_ComponentMgr()->InitTemplateInfo(
            {
                {"Aoi",                             emComponentType::EM_COMPONENT_TYPE_AOI},
                {"test1",                           emComponentType::EM_COMPONENT_TYPE_TESTCOMP_1},
            }
        );

        G_GameObjectMgr()->InitTemplateInfo(
            {
                {"Gameobject",                      emEntityType::EM_ENTITY_TYPE_GAMEOBJECT},
                {"Aoi",                             emEntityType::EM_ENTITY_TYPE_AOI},
                {"gameserver_network",              emEntityType::EM_ENTITY_TYPE_GAMESERVER_NETWORK},
            }
        );
    }
};


BBTATTR_FUNC_DeprecatedMsg("内部变量，不可以调用") 
static EcsInitHelper* __InitHelper = new EcsInitHelper();

} // namespace share::ecs