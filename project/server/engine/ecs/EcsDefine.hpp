#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"


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

typedef int GameObjectId;


}