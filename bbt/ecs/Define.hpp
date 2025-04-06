#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <boost/operators.hpp>
#include <bbt/core/util/ManagerBase.hpp>
#include <bbt/core/reflex/Reflex.hpp>
#include <bbt/ecs/util/Hashmap.hpp>

#define GameApi static inline

#define BBGENGINE_MODULE_NAME "[BBGEngine]"

#define AllSmartDef(classname) \
    typedef std::shared_ptr<classname> classname##SPtr; \
    typedef const std::shared_ptr<classname> classname##CSPtr; \
    typedef std::unique_ptr<classname> classname##UQPtr; \
    typedef const std::unique_ptr<classname> classname##CUQPtr; \
    typedef std::weak_ptr<classname> classname##WKPtr; \
    typedef const std::weak_ptr<classname> classname##CWKPtr; \
    typedef classname* classname##RawPtr;   \
    typedef const classname* classname##CRawPtr

#define OnlySharedDef(classname) \
    typedef std::shared_ptr<classname> classname##SPtr; \
    typedef const std::shared_ptr<classname> classname##CSPtr

#define OnlyUniqueDef(classname) \
    typedef std::unique_ptr<classname> classname##UQPtr; \
    typedef const std::unique_ptr<classname> classname##CUQPtr

#define OnlyWeakDef(classname) \
    typedef std::weak_ptr<classname> classname##WKPtr; \
    typedef const std::weak_ptr<classname> classname##CWKPtr

#define SharedWithUniqueDef(classname) \
    typedef std::shared_ptr<classname> classname##SPtr; \
    typedef const std::shared_ptr<classname> classname##CSPtr; \
    typedef std::weak_ptr<classname> classname##WKPtr; \
    typedef const std::weak_ptr<classname> classname##CWKPtr

    #define GameObjectClassMetaInfo \
    BBTManagerFriendFlag(bbt::ecs::EntityId, bbt::ecs::Entity)

#define ComponentClassMetaInfo(templateid) \
    BBTManagerFriendFlag(bbt::ecs::ComponentId, bbt::ecs::Component); \
public: \
    static bbt::ecs::ComponentTemplateId GetComponentTemplateId() \
    { \
        return templateid; \
    } \
    virtual void __ComponentMeta_DisableReDefine() final {}



namespace bbt::ecs
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

typedef bbt::core::reflex::TypeId ComponentTemplateId; // 模板id
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



}