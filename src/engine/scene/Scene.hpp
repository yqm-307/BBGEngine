// #pragma once
// #include <bbt/core/templateutil/Noncopyable.hpp>
// #include "engine/ecs/entity/Entity.hpp"
// #include "util/hashmap/Hashmap.hpp"

// namespace engine::scene
// {

// const int Default_Hash_Bucket_Num = 128;


// // class Scene
// // {
// //     typedef std::pair<engine::ecs::EntitySPtr, bool> Result;
// // public:
// //     Scene();
// //     virtual ~Scene() = 0;

// //     virtual void        Update() final;
// //     virtual bool        MountGameObject(engine::ecs::EntitySPtr) final;
// //     virtual Result      UnMountGameObject(engine::ecs::EntitySPtr) final; 
// //     virtual Result      UnMountGameObject(engine::ecs::GameObjectId) final;
// //     virtual size_t      GetChildNum() final;
// //     virtual std::vector<engine::ecs::EntitySPtr>
// //                         GetGameObject(engine::ecs::GameObjectTemplateId tid) final;
// // protected:
// //     virtual void        OnUpdate() = 0;

// //     /* 场景根节点的 游戏对象数组 */
// //     std::vector<engine::ecs::EntitySPtr> m_root_gobjs;
// // };

// }