#pragma once
#include <memory>
#include <bbt/base/templateutil/managerconn/ManagerBase.hpp>
#include "util/typedef/NamespaceType.hpp"
#include <bbt/base/uuid/EasyID.hpp>

#pragma region "==> 宏声明 <=="

#define Managed

#define GameObjectDeriveClassDef \
    BBTManagerFriendFlag(engine::ecs::GameObjectId, engine::ecs::GameObject)

#define ComponentDeriveClassDef \
    BBTManagerFriendFlag(engine::ecs::ComponentId, engine::ecs::Component)

#define G_ComponentMgr \
    engine::ecs::ComponentMgr::GetInstance

#define G_GameObjectMgr\
    engine::ecs::GameObjectMgr::GetInstance

#pragma endregion

namespace engine::ecs
{

class ComponentMgr;
class GameObjectMgr;
class SystemMgr;

class Component;
class GameObject;

template<typename TChildClass>
class System;

SharedWithUniqueDef(Component);
SharedWithUniqueDef(GameObject);

#pragma region "==> ID和ID生成器的声明 <=="

enum EasyIdType {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId = 2,
    EM_Mist_AoiObjectId = 3,
};



typedef int64_t GameObjectId;
static const GameObjectId InvalidGameObjectId = -1;
typedef int64_t ComponentId;
static const GameObjectId InvalidComponentId = -1;

typedef int32_t ComponentTemplateId; // 模板id
typedef int32_t GameObjectTemplateId; // 模板id

extern inline GameObjectId GenerateGameObjectID() 
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, EM_Mist_GameObjectId>::GenerateID() ; }

extern inline bool GameObjectIDInvalid(GameObjectId id)
{ return (id <= 0); }

extern inline ComponentId GenerateComponentID() 
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, EM_Mist_ComponentId>::GenerateID(); }

extern inline bool ComponentIDInvalid(ComponentId id)
{ return (id <= 0); }

#pragma endregion




}