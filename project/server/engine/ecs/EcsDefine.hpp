#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"
#include <bbt/uuid/EasyID.hpp>

namespace game::share::ecs
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

typedef int64_t GameObjectId;

extern inline GameObjectId GenerateGameObjectID() {  return bbt::uuid::MistID::GetID_Mist(); }
}