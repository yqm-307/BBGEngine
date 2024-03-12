#pragma once
#include "engine/ecs/EcsDefine.hpp"
#include "util/assert/Assert.hpp"
#include "util/hashmap/Hashmap.hpp"

#define GetGameobjectById(id) engine::ecs::GameObjectMgr::GetInstance()->Search(id)

namespace engine::ecs
{

class GameObjectMgr final:
    public bbt::templateutil::ManagerBase<GameObjectId, GameObject>
{
    typedef std::tuple<std::string, GameObjectTemplateId> GameObjectInfo; 
    typedef std::pair<MemberPtr, bool> Result; 
public:
    ~GameObjectMgr();
    static const std::unique_ptr<GameObjectMgr>& GetInstance();

    virtual Result      Search(KeyType key);
    virtual bool        IsExist(KeyType key);
    std::string  GetName(GameObjectTemplateId tid) const;        

    bool InitTemplateInfo(std::initializer_list<GameObjectInfo> list);
protected:
    GameObjectMgr();

    virtual bool OnMemberCreate(MemberPtr member_base) override;

    virtual bool OnMemberDestory(KeyType key) override;

    virtual KeyType GenerateKey(MemberPtr member_base) override;

private:
    std::map<GameObjectId, GameObjectWKPtr>  m_gameobject_map;
    util::hashmap::Hashmap<ComponentTemplateId, GameObjectInfo, 8> m_gameobject_info_map;
};

}