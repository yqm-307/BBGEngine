#pragma once
// #include "engine/ecs/gameobject/GameObject.hpp"
#include "engine/ecs/EcsDefine.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include "util/assert/Assert.hpp"

namespace engine::ecs
{

class GameObjectMgr:
    public util::managerbase::ManagerBase<GameObjectId, GameObjectSPtr>
{
public:
    ~GameObjectMgr();
    static const std::unique_ptr<GameObjectMgr>& GetInstance();

    virtual Result      Search(KeyType key) override;
    virtual bool        IsExist(KeyType key) override;
    bool                OnInitGameObject(KeyType key, ValueType value);
    bool                OnDestoryGameObject(KeyType key);

    template<typename GameObjectChildType, typename ...InitArgs>
    GameObjectSPtr Create(InitArgs ...args);
protected:
    GameObjectMgr();

private:
    std::map<GameObjectId, GameObjectSPtr>  m_gameobject_map;
};


template<typename GameObjectChildType, typename ...InitArgs>
GameObjectSPtr GameObjectMgr::Create(InitArgs ...args)
{
    static_assert( std::is_base_of_v<GameObject, GameObjectChildType> ); // 必须派生自GameObject

    auto sptr = std::make_shared<GameObjectChildType>(args...);

    sptr->SetId(GenerateGameObjectID());
    bool isok = OnInitGameObject(sptr->GetId(), sptr);

    DebugAssertWithInfo(isok, "repeat key, a little probability event!");
    if(!isok) {
        sptr = nullptr; // 释放对象
        return nullptr;
    }

    return sptr;
}

}