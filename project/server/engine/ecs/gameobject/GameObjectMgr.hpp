#pragma once
#include "engine/ecs/EcsDefine.hpp"
#include "util/assert/Assert.hpp"

#define GetGameobjectById(id) engine::ecs::GameObjectMgr::GetInstance()->Search(id)

namespace engine::ecs
{

class GameObjectMgr:
    public util::managerbase::ManagerBase<GameObjectId, GameObject>
{
public:
    ~GameObjectMgr();
    static const std::unique_ptr<GameObjectMgr>& GetInstance();

    virtual Result      Search(KeyType key) override;
    virtual bool        IsExist(KeyType key) override;
protected:
    GameObjectMgr();

    virtual bool OnMemberCreate(MemberPtr member_base) override;

    virtual bool OnMemberDestory(KeyType key) override;

    virtual KeyType GenerateKey(MemberPtr member_base) override;

private:
    std::map<GameObjectId, GameObjectWKPtr>  m_gameobject_map;
};

}