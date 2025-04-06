#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <boost/operators.hpp>
#include <bbt/core/util/ManagerBase.hpp>
#include <bbt/core/reflex/Reflex.hpp>
#include <util/typedef/NamespaceType.hpp>
#include <util/hashmap/Hashmap.hpp>

#pragma region "==> 宏声明 <=="

#define Managed

#define GameObjectClassMetaInfo \
    BBTManagerFriendFlag(engine::ecs::EntityId, engine::ecs::Entity)

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
class EntityMgr;
class SystemMgr;

class Component;
class Entity;
class System;
class Scene;

class EntityFilter;

SharedWithUniqueDef(Component);
SharedWithUniqueDef(Entity);
SharedWithUniqueDef(System);
SharedWithUniqueDef(Scene);
AllSmartDef(ComponentMgr);
AllSmartDef(EntityMgr);
AllSmartDef(SystemMgr);


#pragma region "==> ID和ID生成器的声明 <=="

enum EasyIdType {
    EM_Mist_GameObjectId = 1,
    EM_Mist_ComponentId,
    EM_Mist_AoiObjectId,
    EM_Mist_TagId,
};



typedef int64_t EntityId;
typedef int64_t ComponentId;
typedef int64_t SystemId;
typedef bbt::core::reflex::TypeId TagId;

static const EntityId InvalidGameObjectId = -1;
static const EntityId InvalidComponentId = -1;
static const EntityId InvalidSystemId = -1;
static const TagId        InvalidTagId = -1;

typedef int32_t ComponentTemplateId; // 模板id
typedef int32_t GameObjectTemplateId; // 模板id

template<int, typename TId>
class EasyId
{
public:
    TId operator()()
    {
        static std::atomic_int64_t id = 0;
        return (TId)(++id);
    };
};

extern inline EntityId GenerateGameObjectID() 
{ return EasyId<EM_Mist_GameObjectId, EntityId>()(); }

extern inline bool GameObjectIDInvalid(EntityId id)
{ return (id <= InvalidGameObjectId); }

extern inline ComponentId GenerateComponentID() 
{ return EasyId<EM_Mist_ComponentId, EntityId>()(); }

extern inline bool ComponentIDInvalid(ComponentId id)
{ return (id <= InvalidComponentId); }

extern inline bool TagIdInvalid(TagId id)
{ return (id <= InvalidTagId); }

#pragma endregion




}