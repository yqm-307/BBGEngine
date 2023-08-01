#pragma once
#include <memory>
#include "util/typedef/NamespaceType.hpp"


namespace game::share::ecs
{

class Component;
class GameObject;
class System;

SmartPtrTypeDef(Component);
SmartPtrTypeDef(GameObject);
SmartPtrTypeDef(System);



}