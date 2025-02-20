#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <boost/operators.hpp>
#include <bbt/base/templateutil/managerconn/ManagerBase.hpp>
#include <bbt/base/templateutil/managerconn/ManagerBase.hpp>
#include <bbt/base/reflex/Reflex.hpp>
#include <bbt/base/uuid/EasyID.hpp>
#include <util/typedef/NamespaceType.hpp>
#include <util/assert/Assert.hpp>
#include <util/hashmap/Hashmap.hpp>

#pragma region "==> 宏声明 <=="

#define Managed

#define GameObjectClassMetaInfo \
    BBTManagerFriendFlag(engine::ecs::GameObjectId, engine::ecs::GameObject)

#define ComponentClassMetaInfo(templateid) \
    BBTManagerFriendFlag(engine::ecs::ComponentId, engine::ecs::Component); \
public: \
    static engine::ecs::ComponentTemplateId GetComponentTemplateId() \
    { \
        return templateid; \
    } \
    virtual void __ComponentMeta_DisableReDefine() final {}

#pragma endregion

namespace engine::ecs
{

class ComponentMgr;
class GameObjectMgr;
class SystemMgr;

class Component;
class GameObject;
class System;
class Scene;

class EntityFilter;

SharedWithUniqueDef(Component);
SharedWithUniqueDef(GameObject);
SharedWithUniqueDef(System);
SharedWithUniqueDef(Scene);
AllSmartDef(ComponentMgr);
AllSmartDef(GameObjectMgr);
AllSmartDef(SystemMgr);


#pragma region "==> ID和ID生成器的声明 <=="

enum EasyIdType {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId,
    EM_Mist_AoiObjectId,
    EM_Mist_TagId,
};



typedef int64_t GameObjectId;
typedef int64_t ComponentId;
typedef int64_t SystemId;
typedef bbt::core::reflex::TypeId TagId;

static const GameObjectId InvalidGameObjectId = -1;
static const GameObjectId InvalidComponentId = -1;
static const GameObjectId InvalidSystemId = -1;
static const TagId        InvalidTagId = -1;

typedef int32_t ComponentTemplateId; // 模板id
typedef int32_t GameObjectTemplateId; // 模板id

extern inline GameObjectId GenerateGameObjectID() 
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, EM_Mist_GameObjectId>::GenerateID() ; }

extern inline bool GameObjectIDInvalid(GameObjectId id)
{ return (id <= InvalidGameObjectId); }

extern inline ComponentId GenerateComponentID() 
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, EM_Mist_ComponentId>::GenerateID(); }

extern inline bool ComponentIDInvalid(ComponentId id)
{ return (id <= InvalidComponentId); }

extern inline bool TagIdInvalid(TagId id)
{ return (id <= InvalidTagId); }

#pragma endregion




}