#pragma once
#include <bbt/core/Attribute.hpp>
#include "engine/ecs/component/ComponentMgr.hpp"
#include "engine/ecs/entity/EntityMgr.hpp"


namespace plugin::ecs
{

enum emEntityType: engine::ecs::GameObjectTemplateId {

//------------------------> 无实体对象 <------------------------//
    EM_ENTITY_TYPE_GAMEOBJECT               = 0,    // 空对象
    EM_ENTITY_TYPE_AOI                      = 1,    // aoi
    EM_ENTITY_TYPE_PLAYER_MGR               = 2,    // 玩家管理器
    EM_ENTITY_TYPE_COMM_SCENE               = 3,    // 通用空场景
    EM_ENTITY_TYPE_NETWORK                  = 4,    //
    EM_ENTITY_TYPE_TIME_WHEEL               = 5,    // timewheel 对象
    EM_ENTITY_TYPE_DB_SERVICE_CLIENT        = 6,    // db service client 对象
    EM_ENTITY_TYPE_DB_GLOBAL_MGR            = 7,    // 全局单例管理者 对象


//------------------------> 实体对象 <------------------------//
    EM_ENTITY_TYPE_HAS_ENTITY               = 1000000,
    EM_ENTITY_TYPE_PLAYER                   = 1000001,      // 玩家
    EM_ENTITY_TYPE_NPC                      = 1000002,      // npc


//------------------------> 测试对象 <------------------------//
    EM_ENTITY_TYPE_TESTOBJ_1                = 1001001,  // 测试对象1

};


enum emComponentType: engine::ecs::ComponentTemplateId {
//------------------------> 特殊组件 <------------------------//
    EM_COMPONENT_TYPE_INVALID               = -1,   // 非法组件
    EM_COMPONENT_TYPE_NONE                  = 0,    // 空组件

//------------------------> 常规组件 <------------------------//
    EM_COMPONENT_TYPE_AOI                   = 1,    // aoi组件
    EM_COMPONENT_TYPE_AOI_OBJECT            = 2,    // aoi object组件
    EM_COMPONENT_TYPE_NETWORK               = 3,    // network 组件
    EM_COMPONENT_TYPE_BUFFLIST              = 4,    // buff list 组件
    EM_COMPONENT_TYPE_ATTRIBUTE             = 5,    // attribute 组件
    EM_COMPONENT_TYPE_GLOBAL_LUA_VM         = 7,    // lua 全局虚拟机
    EM_COMPONENT_TYPE_LUA_TEST              = 8,    // lua 测试组件
    EM_COMPONENT_TYPE_CONN_MGR              = 9,    // conn mgr
    EM_COMPONENT_TYPE_TIME_WHEEL            = 10,   // time wheel
    EM_COMPONENT_TYPE_CLOCK_MODULE          = 11,   // 游戏内时钟
    EM_COMPONENT_TYPE_DBSERVICE_CLIENT      = 12,   // db service client
    EM_COMPONENT_TYPE_SERVER                = 13,   // server
    EM_COMPONENT_TYPE_CLIENT                = 14,   // client

//------------------------> 测试组件 <------------------------//
    EM_COMPONENT_TYPE_TESTCOMP_1            = 50001,  // 测试组件
    EM_COMPONENT_TYPE_EMPTY                 = 50002,   // 空组件
//------------------------> 测试组件 <------------------------//
    EM_COMPONENT_TYPE_MAX_COUNT             = 65535,    // plugin 最大支持组件数量，外部定义请从此id下一位开始
};

class EcsInitHelper
{
public:
    EcsInitHelper(){
        // G_ComponentMgr()->InitTemplateInfo(
        //     {
        //         {"Aoi",                             emComponentType::EM_COMPONENT_TYPE_AOI},
        //         {"test1",                           emComponentType::EM_COMPONENT_TYPE_TESTCOMP_1},
        //         {"npc_buff_list",                   emComponentType::EM_COMPONENT_TYPE_BUFFLIST},
        //         {"npc_attribute",                   emComponentType::EM_COMPONENT_TYPE_ATTRIBUTE},
        //         {"lua_test_script",                 emComponentType::EM_COMPONENT_TYPE_LUA_TEST},
        //         {"connect_manager",                 emComponentType::EM_COMPONENT_TYPE_CONN_MGR},
        //         {"empty_component",                 emComponentType::EM_COMPONENT_TYPE_EMPTY},
        //         {"timewheel",                       emComponentType::EM_COMPONENT_TYPE_TIME_WHEEL},
        //         {"clock_module",                    emComponentType::EM_COMPONENT_TYPE_CLOCK_MODULE},
        //         {"db_service_client",               emComponentType::EM_COMPONENT_TYPE_DBSERVICE_CLIENT},
        //     }
        // );

        // G_GameObjectMgr()->InitTemplateInfo(
        //     {
        //         {"Gameobject",                      emEntityType::EM_ENTITY_TYPE_GAMEOBJECT},
        //         {"Aoi",                             emEntityType::EM_ENTITY_TYPE_AOI},
        //         {"network",                         emEntityType::EM_ENTITY_TYPE_NETWORK},
        //         {"timewheel_obj",                   emEntityType::EM_ENTITY_TYPE_TIME_WHEEL},
        //         {"global_mgr",                      emEntityType::EM_ENTITY_TYPE_DB_GLOBAL_MGR},
        //     }
        // );
    }
};


BBTATTR_FUNC_DeprecatedMsg("内部变量，不可以调用") 
static EcsInitHelper* __InitHelper = new EcsInitHelper();

/**
 * @brief 保存全局对象
 * 
 */
extern engine::ecs::EntitySPtr g_global_obj;

} // namespace share::ecs