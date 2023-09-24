#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"
#include <bbt/uuid/EasyID.hpp>

namespace engine::ecs
{

class ComponentMgr;
class GameObjectMgr;
class SystemMgr;

class Component;
class GameObject;
class System;

SmartPtrTypeDef(Component);
SmartPtrTypeDef(GameObject);
SmartPtrTypeDef(System);

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

}