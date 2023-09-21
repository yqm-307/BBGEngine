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

enum MistKey {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId = 2,
};

typedef int64_t GameObjectId;
typedef int64_t ComponentId;

extern inline GameObjectId GenerateGameObjectID() {  return bbt::uuid::MistID::GetID_Mist<EM_Mist_GameObjectId>(); }
extern inline ComponentId GenerateComponentID() { return bbt::uuid::MistID::GetID_Mist<EM_Mist_ComponentId>(); }
}