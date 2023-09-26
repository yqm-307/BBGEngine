#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"
#include <bbt/uuid/EasyID.hpp>

#pragma region "==> 宏声明 <=="

#define GameObjectDeriveClassDef \
    friend engine::ecs::GameObjectMgr

#pragma endregion

namespace engine::ecs
{

class ComponentMgr;
class GameObjectMgr;
class SystemMgr;

class Component;
class GameObject;
class System;

OnlySharedDef(Component);
SharedWithUniqueDef(GameObject);
OnlySharedDef(System);

#pragma region "==> ID和ID生成器的声明 <=="

enum MistKey {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId = 2,
};



typedef int64_t GameObjectId;
typedef int64_t ComponentId;

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