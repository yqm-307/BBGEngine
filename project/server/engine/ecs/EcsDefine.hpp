#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include <bbt/uuid/EasyID.hpp>

#pragma region "==> 宏声明 <=="

#define Managed

#define GameObjectDeriveClassDef \
    FlagManagedByManagerBase(engine::ecs::GameObjectId, engine::ecs::GameObject)

#define ComponentDeriveClassDef \
    FlagManagedByManagerBase(engine::ecs::ComponentId, engine::ecs::Component)

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
class System;

SharedWithUniqueDef(Component);
SharedWithUniqueDef(GameObject);
SharedWithUniqueDef(System);

#pragma region "==> ID和ID生成器的声明 <=="

enum MistKey {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId = 2,
};



typedef int64_t GameObjectId;
typedef int64_t ComponentId;
typedef int32_t ComponentTemplateId; // 模板id

extern inline GameObjectId GenerateGameObjectID() 
{ return bbt::uuid::EasyID<bbt::uuid::EM_AUTO_INCREMENT, EM_Mist_GameObjectId>::GenerateID() ; }

extern inline bool GameObjectIDInvalid(GameObjectId id)
{ return (id <= 0); }

extern inline ComponentId GenerateComponentID() 
{ return bbt::uuid::EasyID<bbt::uuid::EM_AUTO_INCREMENT, EM_Mist_ComponentId>::GenerateID(); }

extern inline bool ComponentIDInvalid(ComponentId id)
{ return (id <= 0); }

#pragma endregion




}